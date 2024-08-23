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
    char status[MAX_STRING_LENGTH];
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
    fprintf(file, "%s|%s|%s|%s|%s\n", application->date, application->company, application->jobTitle, application->applicationMethod, application->status);
    fclose(file);
}

void saveAllApplicationsToFile(InternshipApplication *applications, int count) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s|%s|%s|%s|%s\n", applications[i].date, applications[i].company, applications[i].jobTitle, applications[i].applicationMethod, applications[i].status);
    }
    fclose(file);
}

void loadApplicationsFromFile(InternshipApplication *applications, int *count) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        return;
    }
    
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
                  applications[*count].date, 
                  applications[*count].company, 
                  applications[*count].jobTitle, 
                  applications[*count].applicationMethod,
                  applications[*count].status) != EOF) {
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
        return yyyy2 - yyyy1; 
    } else if (mm1 != mm2) {
        return mm2 - mm1; 
    } else {
        return dd2 - dd1; 
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

    strcpy(applications[*count].status, "Pending"); 

    saveApplicationToFile(&applications[*count]);

    (*count)++;
}

void updateApplicationStatus(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("No applications to update.\n");
        return;
    }

    char companyName[MAX_STRING_LENGTH];
    printf("Enter the company name of the application you want to update: ");
    fgets(companyName, MAX_STRING_LENGTH, stdin);
    companyName[strcspn(companyName, "\n")] = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(applications[i].company, companyName) == 0) {
            int statusChoice;
            printf("Select the new status:\n");
            printf("1. Accepted\n");
            printf("2. Rejected\n");
            printf("3. Pending\n");
            printf("Enter the number of your choice: ");
            scanf("%d", &statusChoice);
            getchar();

            switch (statusChoice) {
                case 1:
                    strcpy(applications[i].status, "Accepted");
                    break;
                case 2:
                    strcpy(applications[i].status, "Rejected");
                    break;
                case 3:
                    strcpy(applications[i].status, "Pending");
                    break;
                default:
                    printf("Invalid choice. Status not updated.\n");
                    return;
            }

            saveAllApplicationsToFile(applications, count);
            printf("Application status updated successfully.\n");
            return;
        }
    }

    printf("No application found for the specified company.\n");
}

void deleteApplication(InternshipApplication *applications, int *count) {
    if (*count == 0) {
        printf("No applications to delete.\n");
        return;
    }

    int deleteChoice;
    printf("Do you want to delete:\n");
    printf("1. One specific application\n");
    printf("2. All applications\n");
    printf("Enter the number of your choice: ");
    scanf("%d", &deleteChoice);
    getchar();

    if (deleteChoice == 1) {
        char companyName[MAX_STRING_LENGTH];
        char jobTitle[MAX_STRING_LENGTH];
        printf("Enter the company name of the application you want to delete: ");
        fgets(companyName, MAX_STRING_LENGTH, stdin);
        companyName[strcspn(companyName, "\n")] = 0;
        printf("Enter the job title of the application you want to delete: ");
        fgets(jobTitle, MAX_STRING_LENGTH, stdin);
        jobTitle[strcspn(jobTitle, "\n")] = 0;

        for (int i = 0; i < *count; i++) {
            if (strcmp(applications[i].company, companyName) == 0 && strcmp(applications[i].jobTitle, jobTitle) == 0) {
                char confirm[MAX_STRING_LENGTH];
                printf("Are you sure you want to delete this application? (yes/no): ");
                fgets(confirm, MAX_STRING_LENGTH, stdin);
                confirm[strcspn(confirm, "\n")] = 0;

                if (strcmp(confirm, "yes") == 0) {
                    for (int j = i; j < *count - 1; j++) {
                        applications[j] = applications[j + 1];
                    }
                    (*count)--;
                    saveAllApplicationsToFile(applications, *count);
                    printf("Application deleted successfully.\n");
                    return;
                } else {
                    printf("Deletion cancelled.\n");
                    return;
                }
            }
        }
        printf("No matching application found to delete.\n");

    } else if (deleteChoice == 2) {
        char confirm[MAX_STRING_LENGTH];
        printf("Are you sure you want to delete all applications? (yes/no): ");
        fgets(confirm, MAX_STRING_LENGTH, stdin);
        confirm[strcspn(confirm, "\n")] = 0;

        if (strcmp(confirm, "yes") == 0) {
            *count = 0;
            saveAllApplicationsToFile(applications, *count);
            printf("All applications deleted successfully.\n");
        } else {
            printf("Deletion cancelled.\n");
        }
    } else {
        printf("Invalid choice.\n");
    }
}

void displayApplications(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("No applications to display.\n");
        return;
    }

    qsort(applications, count, sizeof(InternshipApplication), compareDates);

    printf("\nAll Applications:\n");
    for (int i = 0; i < count; i++) {
        printf("Date Applied: %s\n", applications[i].date);
        printf("Company Name: %s\n", applications[i].company);
        printf("Job Title: %s\n", applications[i].jobTitle);
        printf("Application Method: %s\n", applications[i].applicationMethod);
        printf("Status: %s\n", applications[i].status);
        printf("------------------------------------------------------------\n");
    }
}

void searchApplications(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("No applications available to search.\n");
        return;
    }

    int searchChoice;
    char searchTerm[MAX_STRING_LENGTH];

    printf("Search by:\n");
    printf("1. Company Name\n");
    printf("2. Job Title\n");
    printf("3. Date Applied\n");
    printf("4. Status\n");
    printf("Enter the number of your choice: ");
    scanf("%d", &searchChoice);
    getchar(); 

    printf("Enter the search term: ");
    fgets(searchTerm, MAX_STRING_LENGTH, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0; 

    int found = 0;

    for (int i = 0; i < count; i++) {
        int match = 0;

        switch (searchChoice) {
            case 1:
                if (strstr(applications[i].company, searchTerm) != NULL) {
                    match = 1;
                }
                break;
            case 2:
                if (strstr(applications[i].jobTitle, searchTerm) != NULL) {
                    match = 1;
                }
                break;
            case 3:
                if (strstr(applications[i].date, searchTerm) != NULL) {
                    match = 1;
                }
                break;
            case 4:
                if (strstr(applications[i].status, searchTerm) != NULL) {
                    match = 1;
                }
                break;
            default:
                printf("Invalid search choice.\n");
                return;
        }

        if (match) {
            printf("\nMatching Application Found:\n");
            printf("Date Applied: %s\n", applications[i].date);
            printf("Company Name: %s\n", applications[i].company);
            printf("Job Title: %s\n", applications[i].jobTitle);
            printf("Application Method: %s\n", applications[i].applicationMethod);
            printf("Status: %s\n", applications[i].status);
            printf("------------------------------------------------------------\n");
            found = 1;
        }
    }

    if (!found) {
        printf("No matching applications found.\n");
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
        printf("3. Update Application Status\n");
        printf("4. Delete Application(s)\n");
        printf("5. Search Applications\n");
        printf("6. Exit\n");
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
                updateApplicationStatus(applications, count);
                break;
            case 4:
                deleteApplication(applications, &count);
                break;
            case 5:
                searchApplications(applications, count);
                break;
            case 6:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
