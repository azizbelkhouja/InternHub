#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_APPLICATIONS 100
#define MAX_STRING_LENGTH 100
#define FILENAME "applications.txt"

typedef struct {
    char date[MAX_STRING_LENGTH];
    char company[MAX_STRING_LENGTH];
    char jobTitle[MAX_STRING_LENGTH];
    char applicationMethod[MAX_STRING_LENGTH];
} InternshipApplication;

int isValidDate(char *date) {
    int dd, mm, yyyy;

    if (sscanf(date, "%d/%d/%d", &dd, &mm, &yyyy) != 3)
        return 0;

    if (yyyy < 2000 || yyyy > 2025)
        return 0;
    if (mm < 1 || mm > 12)
        return 0;
    if (dd < 1 || dd > 31)
        return 0;

    return 1;
}

void saveApplicationToFile(InternshipApplication *application) {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fprintf(file, "%s|%s|%s|%s\n", application->date, application->company, application->jobTitle, application->applicationMethod);
    fclose(file);
}

void loadApplicationsFromFile(InternshipApplication *applications, int *count) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        return;
    }
    
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                  applications[*count].date, 
                  applications[*count].company, 
                  applications[*count].jobTitle, 
                  applications[*count].applicationMethod) != EOF) {
        (*count)++;
    }

    fclose(file);
}

int compareDates(const void *a, const void *b) {
    InternshipApplication *app1 = (InternshipApplication *)a;
    InternshipApplication *app2 = (InternshipApplication *)b;
    
    int dd1, mm1, yyyy1;
    int dd2, mm2, yyyy2;
    
    sscanf(app1->date, "%d/%d/%d", &dd1, &mm1, &yyyy1);
    sscanf(app2->date, "%d/%d/%d", &dd2, &mm2, &yyyy2);
    
    if (yyyy1 != yyyy2) {
        return yyyy2 - yyyy1; // Compare years
    } else if (mm1 != mm2) {
        return mm2 - mm1; // Compare months
    } else {
        return dd2 - dd1; // Compare days
    }
}

void addApplication(InternshipApplication *applications, int *count) {
    if (*count >= MAX_APPLICATIONS) {
        printf("Maximum number of applications reached.\n");
        return;
    }

    do {
        printf("Enter the date you applied (dd/mm/yyyy): ");
        fgets(applications[*count].date, MAX_STRING_LENGTH, stdin);
        applications[*count].date[strcspn(applications[*count].date, "\n")] = 0;
        
        if (!isValidDate(applications[*count].date)) {
            printf("Invalid date format or value. Please try again.\n");
        }
    } while (!isValidDate(applications[*count].date));

    printf("Enter the company name: ");
    fgets(applications[*count].company, MAX_STRING_LENGTH, stdin);
    applications[*count].company[strcspn(applications[*count].company, "\n")] = 0;

    printf("Enter the job title: ");
    fgets(applications[*count].jobTitle, MAX_STRING_LENGTH, stdin);
    applications[*count].jobTitle[strcspn(applications[*count].jobTitle, "\n")] = 0;

    int methodChoice;
    printf("Select the application method:\n");
    printf("1. LinkedIn\n");
    printf("2. Phone\n");
    printf("3. Cold Mail\n");
    printf("4. Indeed\n");
    printf("5. Almalaurea\n");
    printf("Enter the number of your choice: ");
    scanf("%d", &methodChoice);
    getchar(); 

    switch (methodChoice) {
        case 1:
            strcpy(applications[*count].applicationMethod, "LinkedIn");
            break;
        case 2:
            strcpy(applications[*count].applicationMethod, "Phone");
            break;
        case 3:
            strcpy(applications[*count].applicationMethod, "Cold Mail");
            break;
        case 4:
            strcpy(applications[*count].applicationMethod, "Indeed");
            break;
        case 5:
            strcpy(applications[*count].applicationMethod, "Almalaurea");
            break;
        default:
            printf("Invalid choice. Defaulting to 'Unknown'.\n");
            strcpy(applications[*count].applicationMethod, "Unknown");
            break;
    }

    saveApplicationToFile(&applications[*count]);

    (*count)++;
}

void displayApplications(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("No applications to display.\n");
        return;
    }

    qsort(applications, count, sizeof(InternshipApplication), compareDates);

    printf("\nInternship Applications (Sorted by Date - Latest to Earliest):\n");
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

    loadApplicationsFromFile(applications, &count);

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
