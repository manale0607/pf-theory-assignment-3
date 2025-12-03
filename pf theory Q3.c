#include <stdio.h>
#include <string.h>
struct Employee {
    int id;
    char name[50];
    char designation[50];
    float salary;
};
void displayEmployees(struct Employee emp[], int n) {
    printf("\n------------------- Employee Records -------------------\n");
    printf("ID\tName\t\tDesignation\t\tSalary\n");
    printf("--------------------------------------------------------\n");
    int i;
    for (i = 0; i < n; i++) {
        printf("%d\t%-10s\t%-15s\t%.2f\n", 
               emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
    }
}
void findHighestSalary(struct Employee emp[], int n) {
    int index = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (emp[i].salary > emp[index].salary) {
            index = i;
        }
    }

    printf("\nEmployee with Highest Salary:\n");
    printf("ID: %d\n", emp[index].id);
    printf("Name: %s\n", emp[index].name);
    printf("Designation: %s\n", emp[index].designation);
    printf("Salary: %.2f\n", emp[index].salary);
}
void searchEmployee(struct Employee emp[], int n) {
    int choice, searchID;
    char searchName[50];
    int found = 0;

    printf("\nSearch Employee By:\n1. ID\n2. Name\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) { 
        printf("Enter Employee ID: ");
        scanf("%d", &searchID);
        int i;
        for (i = 0; i < n; i++) {
            if (emp[i].id == searchID) {
                printf("\nEmployee Found:\n");
                printf("ID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                       emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
                found = 1;
                break;
            }
        }
    }
    else if (choice == 2) { 
        printf("Enter Employee Name: ");
        scanf("%s", searchName);
        int i;
        for (i = 0; i < n; i++) {
            if (strcmp(emp[i].name, searchName) == 0) {
                printf("\nEmployee Found:\n");
                printf("ID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
                       emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
                found = 1;
                break;
            }
        }
    }
    if (!found)
        printf("\nEmployee not found!\n");
}
void applySalaryBonus(struct Employee emp[], int n, float threshold) {
	int i;
    for (i = 0; i < n; i++) {
        if (emp[i].salary < threshold) {
            emp[i].salary = emp[i].salary + (emp[i].salary * 0.10);
        }
    }
    printf("\n10%% Salary Bonus Applied to Salaries Below %.2f\n", threshold);
}
int main() {
    int n;

    printf("Enter number of employees: ");
    scanf("%d", &n);

    struct Employee emp[n];
    int i;
    for (i = 0; i < n; i++) {
        printf("\nEnter details for Employee %d:\n", i + 1);
        printf("ID: ");
        scanf("%d", &emp[i].id);

        printf("Name: ");
        scanf("%s", emp[i].name);

        printf("Designation: ");
        scanf("%s", emp[i].designation);
        
        printf("Salary: ");
        scanf("%f", &emp[i].salary);
    }
    displayEmployees(emp, n);
    findHighestSalary(emp, n);
    searchEmployee(emp, n);

    applySalaryBonus(emp, n, 50000);
    displayEmployees(emp, n); 

    return 0;
}

