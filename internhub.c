#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_APPLICATIONS 100
#define MAX_STRING_LENGTH 100

typedef struct {
    char date[MAX_STRING_LENGTH];
    char company[MAX_STRING_LENGTH];
    char jobTitle[MAX_STRING_LENGTH];
    char applicationMethod[MAX_STRING_LENGTH];
} InternshipApplication;

void addApplication(InternshipApplication *applications, int *count) {
    if (*count >= MAX_APPLICATIONS) {
        printf("Maximum number of applications reached.\n");
        return;
    }

    printf("Enter the date you applied (e.g., 2024-08-22): ");
    fgets(applications[*count].date, MAX_STRING_LENGTH, stdin);
    applications[*count].date[strcspn(applications[*count].date, "\n")] = 0;

    printf("Enter the company name: ");
    fgets(applications[*count].company, MAX_STRING_LENGTH, stdin);
    applications[*count].company[strcspn(applications[*count].company, "\n")] = 0;

    printf("Enter the job title: ");
    fgets(applications[*count].jobTitle, MAX_STRING_LENGTH, stdin);
    applications[*count].jobTitle[strcspn(applications[*count].jobTitle, "\n")] = 0;

    printf("Enter the application method (LinkedIn, Phone, Cold Mail, Indeed, Almalaurea): ");
    fgets(applications[*count].applicationMethod, MAX_STRING_LENGTH, stdin);
    applications[*count].applicationMethod[strcspn(applications[*count].applicationMethod, "\n")] = 0;

    (*count)++;
}

void displayApplications(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("No applications to display.\n");
        return;
    }

    printf("\nInternship Applications:\n");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("Application %d:\n", i + 1);
        printf("Date Applied: %s\n", applications[i].date);
        printf("Company Name: %s\n", applications[i].company);
        printf("Job Title: %s\n", applications[i].jobTitle);
        printf("Application Method: %s\n", applications[i].applicationMethod);
        printf("------------------------------------------------------------\n");
    }
}

int main() {
    InternshipApplication applications[MAX_APPLICATIONS];
    int count = 0;
    int choice;

    while (1) {
        printf("\nInternship Application Tracker\n");
        printf("1. Add New Application\n");
        printf("2. Display All Applications\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addApplication(applications, &count);
                break;
            case 2:
                displayApplications(applications, count);
                break;
            case 3:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
