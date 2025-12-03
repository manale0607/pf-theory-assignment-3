#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DB_FILENAME "members.dat"
#define INITIAL_CAPACITY 8

typedef struct {
    int id;                     
    char fullName[100];        
    char batch[20];             
    char membership[10];        
    char regDate[11];           
    char dob[11];               
    char interest[6];           
} Student;


void readLineTrim(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len && buf[len-1] == '\n') buf[len-1] = '\0';
}

int findIndexById(Student *db, int count, int id) {
	int i;
    for (i = 0; i < count; ++i) {
        if (db[i].id == id) return i;
    }
    return -1;
}

void printStudent(const Student *s) {
    printf("ID: %d\n", s->id);
    printf("Name: %s\n", s->fullName);
    printf("Batch: %s\n", s->batch);
    printf("Membership: %s\n", s->membership);
    printf("Registration Date: %s\n", s->regDate);
    printf("Date of Birth: %s\n", s->dob);
    printf("Interest: %s\n", s->interest);
    printf("-------------------------------------\n");
}

Student *loadDatabase(const char *filename, int *count, int *capacity) {
    *count = 0;
    *capacity = INITIAL_CAPACITY;
    Student *db = malloc((*capacity) * sizeof(Student));
    if (!db) {
        perror("malloc");
        *capacity = 0;
        return NULL;
    }

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        if (errno == ENOENT) {
            return db;
        } else {
            perror("fopen");
            free(db);
            *capacity = 0;
            return NULL;
        }
    }

    Student tmp;
    while (fread(&tmp, sizeof(Student), 1, fp) == 1) {
        if (*count >= *capacity) {
            *capacity *= 2;
            Student *tmpdb = realloc(db, (*capacity) * sizeof(Student));
            if (!tmpdb) {
                perror("realloc");
                fclose(fp);
                free(db);
                *count = 0;
                *capacity = 0;
                return NULL;
            }
            db = tmpdb;
        }
        db[*count] = tmp;
        (*count)++;
    }

    if (ferror(fp)) {
        perror("fread");
        fclose(fp);
        free(db);
        *count = 0;
        *capacity = 0;
        return NULL;
    }

    fclose(fp);
    return db;
}

int saveDatabase(const char *filename, Student *db, int count) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return 0;
    }
    if (count > 0) {
        size_t written = fwrite(db, sizeof(Student), count, fp);
        if ((int)written != count) {
            perror("fwrite");
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

int addStudentToFile(const char *filename, const Student *s) {
    FILE *fp = fopen(filename, "ab");
    if (!fp) {
        perror("fopen");
        return 0;
    }
    if (fwrite(s, sizeof(Student), 1, fp) != 1) {
        perror("fwrite");
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return 1;
}

int ensureCapacity(Student **dbptr, int *capacity, int minNeeded) {
    if (*capacity >= minNeeded) return 1;
    int newCap = *capacity;
    while (newCap < minNeeded) newCap *= 2;
    Student *tmp = realloc(*dbptr, newCap * sizeof(Student));
    if (!tmp) {
        perror("realloc");
        return 0;
    }
    *dbptr = tmp;
    *capacity = newCap;
    return 1;
}


int addStudent(Student **dbptr, int *count, int *capacity, const Student *s, const char *filename) {
    if (findIndexById(*dbptr, *count, s->id) != -1) {
        printf("Error: Student ID %d already exists.\n", s->id);
        return 0;
    }
    if (!ensureCapacity(dbptr, capacity, (*count) + 1)) return 0;
    (*dbptr)[*count] = *s;
    (*count)++;

    if (!addStudentToFile(filename, s)) {
        printf("Warning: could not append to file. Attempting to save full database.\n");
        if (!saveDatabase(filename, *dbptr, *count)) {
            printf("Save failed.\n");
            return 0;
        }
    }
    return 1;
}

int updateStudent(Student *db, int count, int studentID, const char *filename) {
    int idx = findIndexById(db, count, studentID);
    if (idx == -1) {
        printf("Student ID %d not found.\n", studentID);
        return 0;
    }
    Student *s = &db[idx];
    printf("Current record:\n");
    printStudent(s);

    char choice[4];
    printf("Update (1) Batch or (2) Membership or (3) Both? Enter 1/2/3: ");
    readLineTrim(choice, sizeof(choice));
    if (choice[0] == '1') {
        printf("Enter new Batch (CS / SE / Cyber Security / AI): ");
        readLineTrim(s->batch, sizeof(s->batch));
    } else if (choice[0] == '2') {
        printf("Enter new Membership (IEEE / ACM): ");
        readLineTrim(s->membership, sizeof(s->membership));
    } else if (choice[0] == '3') {
        printf("Enter new Batch (CS / SE / Cyber Security / AI): ");
        readLineTrim(s->batch, sizeof(s->batch));
        printf("Enter new Membership (IEEE / ACM): ");
        readLineTrim(s->membership, sizeof(s->membership));
    } else {
        printf("No update performed.\n");
        return 0;
    }

    if (!saveDatabase(filename, db, count)) {
        printf("Error: failed to save database after update.\n");
        return 0;
    }
    printf("Record updated and saved.\n");
    return 1;
}

int deleteStudent(Student **dbptr, int *count, int studentID, const char *filename) {
    int idx = findIndexById(*dbptr, *count, studentID);
    if (idx == -1) {
        printf("Student ID %d not found.\n", studentID);
        return 0;
    }
    int i;
    for (i = idx; i < (*count) - 1; ++i) {
        (*dbptr)[i] = (*dbptr)[i + 1];
    }
    (*count)--;

    if (!saveDatabase(filename, *dbptr, *count)) {
        printf("Error: failed to save database after deletion.\n");
        return 0;
    }
    printf("Student ID %d deleted and database saved.\n", studentID);
    return 1;
}

void displayAll(Student *db, int count) {
    if (count == 0) {
        printf("No records to display.\n");
        return;
    }
    printf("\n--- All Student Records (%d) ---\n", count);
    int i;
    for (i = 0; i < count; ++i) {
        printStudent(&db[i]);
    }
}
void generateBatchReport(Student *db, int count) {
    char batch[20];
    char membership[10];
    printf("Enter Batch (CS / SE / Cyber Security / AI): ");
    readLineTrim(batch, sizeof(batch));
    printf("Filter Membership (IEEE / ACM / Both / All): ");
    readLineTrim(membership, sizeof(membership));

    printf("\n--- Report for Batch: %s  Membership: %s ---\n", batch, membership);
    int found = 0;
    int i;
    for (i = 0; i < count; ++i) {
        int batchMatch = (strcmp(db[i].batch, batch) == 0);
        int memMatch = 0;
        if (strcmp(membership, "All") == 0) memMatch = 1;
        else if (strcmp(membership, "Both") == 0) {
            memMatch = (strcmp(db[i].interest, "Both") == 0);
        } else {
            memMatch = (strcmp(db[i].membership, membership) == 0) || (strcmp(db[i].interest, membership) == 0) || (strcmp(db[i].interest, "Both") == 0 && strcmp(db[i].membership, membership) == 0);
        }
        if (batchMatch && memMatch) {
            printStudent(&db[i]);
            found = 1;
        }
    }
    if (!found) printf("No matching records found.\n");
}



void registerNewStudent(Student **dbptr, int *count, int *capacity, const char *filename) {
    Student s;
    char tmp[128];

    printf("Enter Student ID (integer): ");
    if (scanf("%d", &s.id) != 1) {
        while (getchar() != '\n'); 
        printf("Invalid ID.\n");
        return;
    }
    getchar();

    if (findIndexById(*dbptr, *count, s.id) != -1) {
        printf("Error: Student ID %d already exists. Registration aborted.\n", s.id);
        return;
    }

    printf("Enter Full Name: ");
    readLineTrim(s.fullName, sizeof(s.fullName));

    printf("Enter Batch (CS / SE / Cyber Security / AI): ");
    readLineTrim(s.batch, sizeof(s.batch));

    printf("Enter Membership (IEEE / ACM): ");
    readLineTrim(s.membership, sizeof(s.membership));

    printf("Enter Registration Date (YYYY-MM-DD): ");
    readLineTrim(s.regDate, sizeof(s.regDate));

    printf("Enter Date of Birth (YYYY-MM-DD): ");
    readLineTrim(s.dob, sizeof(s.dob));

    printf("Interest (IEEE / ACM / Both): ");
    readLineTrim(s.interest, sizeof(s.interest));

    if (addStudent(dbptr, count, capacity, &s, filename)) {
        printf("Student ID %d registered successfully.\n", s.id);
    } else {
        printf("Failed to register student.\n");
    }
}

int main(void) {
    int count = 0, capacity = 0;
    Student *db = loadDatabase(DB_FILENAME, &count, &capacity);
    if (!db) {
        printf("Failed to initialize database. Exiting.\n");
        return 1;
    }
    printf("Loaded %d records from %s (capacity %d)\n", count, DB_FILENAME, capacity);

    int choice;
    while (1) {
        printf("\n--- IEEE/ACM Membership Manager ---\n");
        printf("1. Register new student\n");
        printf("2. Update student's batch/membership\n");
        printf("3. Delete a student\n");
        printf("4. View all registrations\n");
        printf("5. Generate batch-wise report\n");
        printf("6. Save database now\n");
        printf("7. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); 
            printf("Invalid input.\n");
            continue;
        }
        getchar(); 
        if (choice == 1) {
            registerNewStudent(&db, &count, &capacity, DB_FILENAME);
        } else if (choice == 2) {
            int id;
            printf("Enter Student ID to update: ");
            if (scanf("%d", &id) != 1) { while(getchar()!='\n'); printf("Invalid ID.\n"); continue; }
            getchar();
            updateStudent(db, count, id, DB_FILENAME);
        } else if (choice == 3) {
            int id;
            printf("Enter Student ID to delete: ");
            if (scanf("%d", &id) != 1) { while(getchar()!='\n'); printf("Invalid ID.\n"); continue; }
            getchar();
            deleteStudent(&db, &count, id, DB_FILENAME);
        } else if (choice == 4) {
            displayAll(db, count);
        } else if (choice == 5) {
            generateBatchReport(db, count);
        } else if (choice == 6) {
            if (saveDatabase(DB_FILENAME, db, count)) printf("Database saved.\n");
            else printf("Failed to save database.\n");
        } else if (choice == 7) {
            if (!saveDatabase(DB_FILENAME, db, count)) {
                printf("Warning: failed to save database before exit.\n");
            } else {
                printf("Database saved. Exiting.\n");
            }
            break;
        } else {
            printf("Unknown choice.\n");
        }
    }

    free(db);
    return 0;
}

