#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_APPLICATIONS 100
#define MAX_STRING_LENGTH 100
#define FILENAME "applications.txt"
#define SHIFT 3

typedef struct {
    char date[MAX_STRING_LENGTH];
    char company[MAX_STRING_LENGTH];
    char jobTitle[MAX_STRING_LENGTH];
    char applicationMethod[MAX_STRING_LENGTH];
    char status[MAX_STRING_LENGTH];
} InternshipApplication;

void encrypt(char *text, int shift) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = islower(text[i]) ? 'a' : 'A';
            text[i] = (text[i] - base + shift) % 26 + base;
        }
    }
}

void decrypt(char *text, int shift) {
    encrypt(text, 26 - shift);
}

int isValidDate(char *date) {
    int dd, mm, yyyy;

    if (sscanf(date, "%d/%d/%d", &dd, &mm, &yyyy) != 3)
        return 0;

    if (yyyy < 2000 || yyyy > 2025)
        return 0;
    
    if (mm < 1 || mm > 12)
        return 0;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (mm == 2 && ((yyyy % 4 == 0 && yyyy % 100 != 0) || (yyyy % 400 == 0))) {
        daysInMonth[1] = 29;
    }

    if (dd < 1 || dd > daysInMonth[mm - 1])
        return 0;

    return 1;
}

int isValidCompanyName(char *company) {
    for (int i = 0; company[i] != '\0'; i++) {
        if (!isalnum(company[i]) && company[i] != ' ' && company[i] != '&' && company[i] != '.') {
            return 0;
        }
    }
    return 1;
}

void saveApplicationToFile(InternshipApplication *application) {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    char encryptedCompany[MAX_STRING_LENGTH];
    char encryptedJobTitle[MAX_STRING_LENGTH];
    char encryptedMethod[MAX_STRING_LENGTH];
    char encryptedStatus[MAX_STRING_LENGTH];
    
    strcpy(encryptedCompany, application->company);
    strcpy(encryptedJobTitle, application->jobTitle);
    strcpy(encryptedMethod, application->applicationMethod);
    strcpy(encryptedStatus, application->status);
    
    encrypt(encryptedCompany, SHIFT);
    encrypt(encryptedJobTitle, SHIFT);
    encrypt(encryptedMethod, SHIFT);
    encrypt(encryptedStatus, SHIFT);
    
    fprintf(file, "%s|%s|%s|%s|%s\n", application->date, encryptedCompany, encryptedJobTitle, encryptedMethod, encryptedStatus);
    fclose(file);
}

void saveAllApplicationsToFile(InternshipApplication *applications, int count) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        char encryptedCompany[MAX_STRING_LENGTH];
        char encryptedJobTitle[MAX_STRING_LENGTH];
        char encryptedMethod[MAX_STRING_LENGTH];
        char encryptedStatus[MAX_STRING_LENGTH];
        
        strcpy(encryptedCompany, applications[i].company);
        strcpy(encryptedJobTitle, applications[i].jobTitle);
        strcpy(encryptedMethod, applications[i].applicationMethod);
        strcpy(encryptedStatus, applications[i].status);
        
        encrypt(encryptedCompany, SHIFT);
        encrypt(encryptedJobTitle, SHIFT);
        encrypt(encryptedMethod, SHIFT);
        encrypt(encryptedStatus, SHIFT);
        
        fprintf(file, "%s|%s|%s|%s|%s\n", applications[i].date, encryptedCompany, encryptedJobTitle, encryptedMethod, encryptedStatus);
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
        decrypt(applications[*count].company, SHIFT);
        decrypt(applications[*count].jobTitle, SHIFT);
        decrypt(applications[*count].applicationMethod, SHIFT);
        decrypt(applications[*count].status, SHIFT);
        
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
        printf("Error: Maximum number of applications reached.\n");
        return;
    }

    do {
        system("cls");
        printf("\nEnter the date of application (format: dd/mm/yyyy): ");
        fgets(applications[*count].date, MAX_STRING_LENGTH, stdin);
        applications[*count].date[strcspn(applications[*count].date, "\n")] = 0;
        
        if (!isValidDate(applications[*count].date)) {
            printf("Invalid date format or value. Please enter the date in the correct format.\n");
            getchar();
        }
    } while (!isValidDate(applications[*count].date));

    do {
        system("cls");
        printf("Enter the company name: ");
        fgets(applications[*count].company, MAX_STRING_LENGTH, stdin);
        applications[*count].company[strcspn(applications[*count].company, "\n")] = 0;

        if (!isValidCompanyName(applications[*count].company)) {
            printf("Invalid company name. Please use only letters, numbers, spaces, '&', and '.'.\n");
            getchar();
        }
    } while (!isValidCompanyName(applications[*count].company));

    printf("Enter the job title: ");
    fgets(applications[*count].jobTitle, MAX_STRING_LENGTH, stdin);
    applications[*count].jobTitle[strcspn(applications[*count].jobTitle, "\n")] = 0;

    int methodChoice;
    printf("\nSelect the application method:\n");
    printf("  1. LinkedIn\n");
    printf("  2. Phone\n");
    printf("  3. Cold Mail\n");
    printf("  4. Indeed\n");
    printf("  5. Almalaurea\n");
    printf("Enter the number corresponding to your choice: ");
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
    system("cls");
    printf("\nApplication added successfully.\n");
}

void updateApplicationStatus(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("\nNo applications to update.\n");
        getchar();
        return;
    }

    char companyName[MAX_STRING_LENGTH];
    char jobTitle[MAX_STRING_LENGTH];
    printf("\nEnter the company name of the application to update: ");
    fgets(companyName, MAX_STRING_LENGTH, stdin);
    companyName[strcspn(companyName, "\n")] = 0;
    printf("Enter the job title of the application to update: ");
    fgets(jobTitle, MAX_STRING_LENGTH, stdin);
    jobTitle[strcspn(jobTitle, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(applications[i].company, companyName) == 0 && strcmp(applications[i].jobTitle, jobTitle) == 0) {
            printf("\nCurrent status: %s\n", applications[i].status);
            printf("Enter new status: ");
            fgets(applications[i].status, MAX_STRING_LENGTH, stdin);
            applications[i].status[strcspn(applications[i].status, "\n")] = 0;
            saveAllApplicationsToFile(applications, count);
            system("cls");
            printf("\nStatus updated successfully.\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("\nNo matching application found to update.\n");
    }
}

void searchApplications(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("\nNo applications to search.\n");
        getchar();
        return;
    }

    int searchChoice;
    system("cls");
    printf("\nSearch options:\n");
    printf("  1. By Company Name\n");
    printf("  2. By Job Title\n");
    printf("Enter the number corresponding to your choice: ");
    scanf("%d", &searchChoice);
    getchar();

    char searchQuery[MAX_STRING_LENGTH];

    if (searchChoice == 1) {
        printf("\nEnter the company name to search: ");
        fgets(searchQuery, MAX_STRING_LENGTH, stdin);
        searchQuery[strcspn(searchQuery, "\n")] = 0;

        printf("\nSearch Results:\n");
        printf("------------------------------------------------------------\n");
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcasecmp(applications[i].company, searchQuery) == 0) {
                printf("Date Applied      : %s\n", applications[i].date);
                printf("Company Name      : %s\n", applications[i].company);
                printf("Job Title         : %s\n", applications[i].jobTitle);
                printf("Application Method: %s\n", applications[i].applicationMethod);
                printf("Status            : %s\n", applications[i].status);
                printf("------------------------------------------------------------\n");
                found = 1;
            }
        }
        if (!found) {
            printf("No matching applications found.\n");
        }

    } else if (searchChoice == 2) {
        printf("\nEnter the job title to search: ");
        fgets(searchQuery, MAX_STRING_LENGTH, stdin);
        searchQuery[strcspn(searchQuery, "\n")] = 0;

        printf("\nSearch Results:\n");
        printf("------------------------------------------------------------\n");
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcasecmp(applications[i].jobTitle, searchQuery) == 0) {
                printf("Date Applied      : %s\n", applications[i].date);
                printf("Company Name      : %s\n", applications[i].company);
                printf("Job Title         : %s\n", applications[i].jobTitle);
                printf("Application Method: %s\n", applications[i].applicationMethod);
                printf("Status            : %s\n", applications[i].status);
                printf("------------------------------------------------------------\n");
                found = 1;
            }
        }
        if (!found) {
            printf("No matching applications found.\n");
        }
    } else {
        printf("\nInvalid choice. Returning to main menu.\n");
    }
}

void displayApplications(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("\nNo applications to display.\n");
        getchar();
        return;
    }

    qsort(applications, count, sizeof(InternshipApplication), compareDates);

    system("cls");
    printf("\nInternship Applications (Sorted by Date - Latest to Earliest):\n");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("Application %d:\n", i + 1);
        printf("Date Applied      : %s\n", applications[i].date);
        printf("Company Name      : %s\n", applications[i].company);
        printf("Job Title         : %s\n", applications[i].jobTitle);
        printf("Application Method: %s\n", applications[i].applicationMethod);
        printf("Status            : %s\n", applications[i].status);
        printf("------------------------------------------------------------\n");
    }
    getchar();
}

void deleteApplication(InternshipApplication *applications, int *count) {
    if (*count == 0) {
        printf("\nNo applications to delete.\n");
        getchar();
        return;
    }

    int deleteChoice;
    system("cls");
    printf("\nDelete options:\n");
    printf("  1. Delete a specific application\n");
    printf("  2. Delete all applications\n");
    printf("Enter the number corresponding to your choice: ");
    scanf("%d", &deleteChoice);
    getchar();

    if (deleteChoice == 1) {
        char companyName[MAX_STRING_LENGTH];
        char jobTitle[MAX_STRING_LENGTH];
        printf("\nEnter the company name of the application to delete: ");
        fgets(companyName, MAX_STRING_LENGTH, stdin);
        companyName[strcspn(companyName, "\n")] = 0;
        printf("Enter the job title of the application to delete: ");
        fgets(jobTitle, MAX_STRING_LENGTH, stdin);
        jobTitle[strcspn(jobTitle, "\n")] = 0;

        for (int i = 0; i < *count; i++) {
            if (strcmp(applications[i].company, companyName) == 0 && strcmp(applications[i].jobTitle, jobTitle) == 0) {
                char confirm[MAX_STRING_LENGTH];
                printf("\nAre you sure you want to delete this application? (yes/no): ");
                fgets(confirm, MAX_STRING_LENGTH, stdin);
                confirm[strcspn(confirm, "\n")] = 0;

                if (strcasecmp(confirm, "yes") == 0) {
                    for (int j = i; j < *count - 1; j++) {
                        applications[j] = applications[j + 1];
                    }
                    (*count)--;
                    saveAllApplicationsToFile(applications, *count);
                    system("cls");
                    printf("\nApplication deleted successfully.\n");
                    return;
                } else {
                    printf("\nDeletion cancelled.\n");
                    return;
                }
            }
        }
        printf("\nNo matching application found to delete.\n");

    } else if (deleteChoice == 2) {
        char confirm[MAX_STRING_LENGTH];
        printf("\nAre you sure you want to delete all applications? (yes/no): ");
        fgets(confirm, MAX_STRING_LENGTH, stdin);
        confirm[strcspn(confirm, "\n")] = 0;

        if (strcasecmp(confirm, "yes") == 0) {
            *count = 0;
            FILE *file = fopen(FILENAME, "w");
            if (file != NULL) {
                fclose(file);
            }
            system("cls");
            printf("\nAll applications deleted successfully.\n");
        } else {
            printf("\nDeletion of all applications cancelled.\n");
        }
    } else {
        printf("\nInvalid choice. Deletion cancelled.\n");
    }
}

int main() {
    InternshipApplication applications[MAX_APPLICATIONS];
    int count = 0;
    int choice;

    loadApplicationsFromFile(applications, &count);

    while (1) {
        system("cls");
        printf("\nInternship Application Tracker\n");
        printf("------------------------------------------------------------\n");
        printf("  1. Add New Application\n");
        printf("  2. Display All Applications\n");
        printf("  3. Update Application Status\n");
        printf("  4. Search Applications\n");
        printf("  5. Delete Application(s)\n");
        printf("  6. Exit\n");
        printf("------------------------------------------------------------\n");
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
                searchApplications(applications, count);
                break;
            case 5:
                deleteApplication(applications, &count);
                break;
            case 6:
                system("cls");
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
                getchar();
        }
    }

    return 0;
}
