#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 5
#define MAX_LINE_LENGTH 1024

void insertLine(char **lines, int *count, int *capacity, int index, const char *text);
void deleteLine(char **lines, int *count, int index);
void printAllLines(char **lines, int count);
void freeAll(char **lines, int count);
void shrinkToFit(char **lines, int *capacity, int count);
void saveToFile(char **lines, int count, const char *filename);
char **loadFromFile(const char *filename, int *count, int *capacity);

int main() {
    int count = 0, capacity = INITIAL_CAPACITY;
    char **lines = malloc(capacity * sizeof(char*));
    if (!lines) {
        perror("Memory allocation failed");
        return 1;
    }

    char buffer[MAX_LINE_LENGTH];
    int choice;

    while (1) {
        printf("\n1. Insert Line\n2. Delete Line\n3. Print All Lines\n4. Shrink to Fit\n5. Save to File\n6. Load from File\n7. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); continue; }
        getchar();

        if (choice == 1) {
            int index;
            printf("Enter line index to insert: ");
            if (scanf("%d", &index) != 1) { while(getchar()!='\n'); continue; }
            getchar();

            printf("Enter text: ");
            fgets(buffer, MAX_LINE_LENGTH, stdin);
            buffer[strcspn(buffer, "\n")] = 0;

            insertLine(lines, &count, &capacity, index, buffer);
        }
        else if (choice == 2) {
            int index;
            printf("Enter line index to delete: ");
            if (scanf("%d", &index) != 1) { while(getchar()!='\n'); continue; }
            getchar();

            deleteLine(lines, &count, index);
        }
        else if (choice == 3) {
            printAllLines(lines, count);
        }
        else if (choice == 4) {
            shrinkToFit(lines, &capacity, count);
        }
        else if (choice == 5) {
            char filename[100];
            printf("Enter filename to save: ");
            scanf("%s", filename);
            saveToFile(lines, count, filename);
        }
        else if (choice == 6) {
            char filename[100];
            printf("Enter filename to load: ");
            scanf("%s", filename);
            freeAll(lines, count);
            lines = loadFromFile(filename, &count, &capacity);
            if (!lines) {
                lines = malloc(INITIAL_CAPACITY * sizeof(char*));
                count = 0;
                capacity = INITIAL_CAPACITY;
            }
        }
        else if (choice == 7) {
            break;
        }
    }

    freeAll(lines, count);
    return 0;
}

// Insert line (auto-append if index > count)
void insertLine(char **lines, int *count, int *capacity, int index, const char *text) {
    if (index < 0) index = 0;
    if (index > *count) index = *count; // append

    if (*count >= *capacity) {
        *capacity *= 2;
        char **tmp = realloc(lines, (*capacity) * sizeof(char*));
        if (!tmp) { perror("Realloc failed"); return; }
        lines = tmp;
    }
    int i;
    for (i = *count; i > index; i--) {
        lines[i] = lines[i-1];
    }

    lines[index] = malloc(strlen(text)+1);
    if (!lines[index]) { perror("Line allocation failed"); return; }
    strcpy(lines[index], text);
    (*count)++;
}

void deleteLine(char **lines, int *count, int index) {
    if (index < 0 || index >= *count) {
        printf("Invalid index.\n");
        return;
    }

    free(lines[index]);
    int i;
    for (i = index; i < *count - 1; i++) {
        lines[i] = lines[i+1];
    }
    (*count)--;
}

void printAllLines(char **lines, int count) {
    printf("\n----- Text Editor Buffer -----\n");
    int i;
    for (i = 0; i < count; i++) {
        printf("%d: %s\n", i, lines[i]);
    }
}

void freeAll(char **lines, int count) {
	int i;
    for (i = 0; i < count; i++) {
        free(lines[i]);
    }
    free(lines);
}

void shrinkToFit(char **lines, int *capacity, int count) {
    char **tmp = realloc(lines, count * sizeof(char*));
    if (!tmp) { perror("Shrink failed"); return; }
    lines = tmp;
    *capacity = count;
    printf("Memory shrunk to fit %d lines.\n", count);
}

void saveToFile(char **lines, int count, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) { perror("File open failed"); return; }
    
    int i;
    for (i = 0; i < count; i++) {
        fprintf(fp, "%s\n", lines[i]);
    }
    fclose(fp);
    printf("Saved %d lines to %s\n", count, filename);
}

char **loadFromFile(const char *filename, int *count, int *capacity) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("File open failed"); return NULL; }

    int cap = INITIAL_CAPACITY, cnt = 0;
    char **lines = malloc(cap * sizeof(char*));
    if (!lines) { perror("Memory allocation failed"); fclose(fp); return NULL; }

    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, MAX_LINE_LENGTH, fp)) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (cnt >= cap) {
            cap *= 2;
            char **tmp = realloc(lines, cap * sizeof(char*));
            if (!tmp) { perror("Realloc failed"); break; }
            lines = tmp;
        }

        lines[cnt] = malloc(strlen(buffer)+1);
        if (!lines[cnt]) { perror("Line allocation failed"); break; }
        strcpy(lines[cnt], buffer);
        cnt++;
    }
    fclose(fp);
    *count = cnt;
    *capacity = cap;
    return lines;
}

