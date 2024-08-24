#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_APPLICATIONS 100
#define MAX_STRING_LENGTH 100
#define FILENAME "applications.txt"
#define BACKUP_FILENAME "backup.txt"
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

void createBackup() {
    FILE *source = fopen(FILENAME, "r");
    if (source == NULL) {
        printf("Error opening source file for backup.\n");
        return;
    }
    FILE *backup = fopen(BACKUP_FILENAME, "w");
    if (backup == NULL) {
        printf("Error opening backup file for writing.\n");
        fclose(source);
        return;
    }

    char ch;
    while ((ch = fgetc(source)) != EOF) {
        fputc(ch, backup);
    }

    fclose(source);
    fclose(backup);
    system("cls");
    printf("Backup created successfully.\n");
}

void restoreFromBackup() {
    FILE *source = fopen(BACKUP_FILENAME, "r");
    if (source == NULL) {
        printf("Error opening backup file for restoration.\n");
        return;
    }
    FILE *destination = fopen(FILENAME, "w");
    if (destination == NULL) {
        printf("Error opening destination file for restoration.\n");
        fclose(source);
        return;
    }

    char ch;
    while ((ch = fgetc(source)) != EOF) {
        fputc(ch, destination);
    }

    fclose(source);
    fclose(destination);
    system("cls");
    printf("Data restored from backup successfully.\n");
}

void displayStatistics(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("\nNo applications to display statistics.\n");
        getchar();
        return;
    }

    int pendingCount = 0;
    int interviewedCount = 0;
    int rejectedCount = 0;
    int acceptedCount = 0;

    int companyCounts[MAX_APPLICATIONS] = {0};
    char companies[MAX_APPLICATIONS][MAX_STRING_LENGTH];
    int companyCount = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(applications[i].status, "Pending") == 0) {
            pendingCount++;
        } else if (strcmp(applications[i].status, "Interviewed") == 0) {
            interviewedCount++;
        } else if (strcmp(applications[i].status, "Rejected") == 0) {
            rejectedCount++;
        } else if (strcmp(applications[i].status, "Accepted") == 0) {
            acceptedCount++;
        }

        int found = 0;
        for (int j = 0; j < companyCount; j++) {
            if (strcmp(companies[j], applications[i].company) == 0) {
                companyCounts[j]++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(companies[companyCount], applications[i].company);
            companyCounts[companyCount] = 1;
            companyCount++;
        }
    }

    system("cls");
    printf("\nStatistics:\n");
    printf("------------------------------------------------------------\n");
    printf("Total Applications: %d\n", count);
    printf("Pending: %d\n", pendingCount);
    printf("Interviewed: %d\n", interviewedCount);
    printf("Rejected: %d\n", rejectedCount);
    printf("Accepted: %d\n", acceptedCount);

    printf("\nMost Applied-to Companies:\n");
    for (int i = 0; i < companyCount; i++) {
        printf("%s: %d\n", companies[i], companyCounts[i]);
    }
    printf("------------------------------------------------------------\n");
    getchar();
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
        printf("Application limit reached.\n");
        getchar();
        return;
    }

    InternshipApplication newApplication;
    printf("Enter the date applied (DD/MM/YYYY): ");
    fgets(newApplication.date, MAX_STRING_LENGTH, stdin);
    newApplication.date[strcspn(newApplication.date, "\n")] = 0;

    if (!isValidDate(newApplication.date)) {
        printf("Invalid date format. Please use DD/MM/YYYY.\n");
        return;
    }

    printf("Enter the company name: ");
    fgets(newApplication.company, MAX_STRING_LENGTH, stdin);
    newApplication.company[strcspn(newApplication.company, "\n")] = 0;

    if (!isValidCompanyName(newApplication.company)) {
        printf("Invalid company name. Please use alphanumeric characters only.\n");
        return;
    }

    printf("Enter the job title: ");
    fgets(newApplication.jobTitle, MAX_STRING_LENGTH, stdin);
    newApplication.jobTitle[strcspn(newApplication.jobTitle, "\n")] = 0;

    printf("Enter the application method: ");
    fgets(newApplication.applicationMethod, MAX_STRING_LENGTH, stdin);
    newApplication.applicationMethod[strcspn(newApplication.applicationMethod, "\n")] = 0;

    printf("Enter the status (Pending/Interviewed/Rejected/Accepted): ");
    fgets(newApplication.status, MAX_STRING_LENGTH, stdin);
    newApplication.status[strcspn(newApplication.status, "\n")] = 0;

    applications[*count] = newApplication;
    (*count)++;
    saveAllApplicationsToFile(applications, *count);
    system("cls");
    printf("Application added successfully.\n");
}

void updateApplicationStatus(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("No applications to update.\n");
        getchar();
        return;
    }

    char companyName[MAX_STRING_LENGTH];
    char jobTitle[MAX_STRING_LENGTH];
    printf("Enter the company name of the application to update: ");
    fgets(companyName, MAX_STRING_LENGTH, stdin);
    companyName[strcspn(companyName, "\n")] = 0;
    printf("Enter the job title of the application to update: ");
    fgets(jobTitle, MAX_STRING_LENGTH, stdin);
    jobTitle[strcspn(jobTitle, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(applications[i].company, companyName) == 0 && strcmp(applications[i].jobTitle, jobTitle) == 0) {
            printf("Current status: %s\n", applications[i].status);
            printf("Enter new status: ");
            fgets(applications[i].status, MAX_STRING_LENGTH, stdin);
            applications[i].status[strcspn(applications[i].status, "\n")] = 0;
            saveAllApplicationsToFile(applications, count);
            system("cls");
            printf("Status updated successfully.\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("No matching application found to update.\n");
    }
}

void searchApplications(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("No applications to search.\n");
        getchar();
        return;
    }

    int searchChoice;
    system("cls");
    printf("Search options:\n");
    printf("  1. By Company Name\n");
    printf("  2. By Job Title\n");
    printf("Enter the number corresponding to your choice: ");
    scanf("%d", &searchChoice);
    getchar();

    char searchQuery[MAX_STRING_LENGTH];

    if (searchChoice == 1) {
        printf("Enter the company name to search: ");
        fgets(searchQuery, MAX_STRING_LENGTH, stdin);
        searchQuery[strcspn(searchQuery, "\n")] = 0;

        printf("Search Results:\n");
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
        printf("Enter the job title to search: ");
        fgets(searchQuery, MAX_STRING_LENGTH, stdin);
        searchQuery[strcspn(searchQuery, "\n")] = 0;

        printf("Search Results:\n");
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
        printf("Invalid choice. Returning to main menu.\n");
    }
}

void displayApplications(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("No applications to display.\n");
        getchar();
        return;
    }

    qsort(applications, count, sizeof(InternshipApplication), compareDates);

    system("cls");
    printf("Internship Applications (Sorted by Date - Latest to Earliest):\n");
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
        printf("No applications to delete.\n");
        getchar();
        return;
    }

    int deleteChoice;
    system("cls");
    printf("Delete options:\n");
    printf("  1. Delete a specific application\n");
    printf("  2. Delete all applications\n");
    printf("Enter the number corresponding to your choice: ");
    scanf("%d", &deleteChoice);
    getchar();

    if (deleteChoice == 1) {
        char companyName[MAX_STRING_LENGTH];
        char jobTitle[MAX_STRING_LENGTH];
        printf("Enter the company name of the application to delete: ");
        fgets(companyName, MAX_STRING_LENGTH, stdin);
        companyName[strcspn(companyName, "\n")] = 0;
        printf("Enter the job title of the application to delete: ");
        fgets(jobTitle, MAX_STRING_LENGTH, stdin);
        jobTitle[strcspn(jobTitle, "\n")] = 0;

        for (int i = 0; i < *count; i++) {
            if (strcmp(applications[i].company, companyName) == 0 && strcmp(applications[i].jobTitle, jobTitle) == 0) {
                char confirm[MAX_STRING_LENGTH];
                printf("Are you sure you want to delete this application? (yes/no): ");
                fgets(confirm, MAX_STRING_LENGTH, stdin);
                confirm[strcspn(confirm, "\n")] = 0;

                if (strcasecmp(confirm, "yes") == 0) {
                    for (int j = i; j < *count - 1; j++) {
                        applications[j] = applications[j + 1];
                    }
                    (*count)--;
                    saveAllApplicationsToFile(applications, *count);
                    system("cls");
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

        if (strcasecmp(confirm, "yes") == 0) {
            *count = 0;
            FILE *file = fopen(FILENAME, "w");
            if (file != NULL) {
                fclose(file);
            }
            system("cls");
            printf("All applications deleted successfully.\n");
        } else {
            printf("Deletion of all applications cancelled.\n");
        }
    } else {
        printf("Invalid choice. Deletion cancelled.\n");
    }
}

int main() {
    InternshipApplication applications[MAX_APPLICATIONS];
    int count = 0;
    int choice;

    loadApplicationsFromFile(applications, &count);

    do {
        system("cls");
        printf("Internship Application Tracker\n");
        printf("1. Add Application\n");
        printf("2. Update Application Status\n");
        printf("3. Search Applications\n");
        printf("4. Display Applications\n");
        printf("5. Delete Application(s)\n");
        printf("6. Create Backup\n");
        printf("7. Restore from Backup\n");
        printf("8. Display Statistics\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addApplication(applications, &count);
                break;
            case 2:
                updateApplicationStatus(applications, count);
                break;
            case 3:
                searchApplications(applications, count);
                break;
            case 4:
                displayApplications(applications, count);
                break;
            case 5:
                deleteApplication(applications, &count);
                break;
            case 6:
                createBackup();
                break;
            case 7:
                restoreFromBackup();
                loadApplicationsFromFile(applications, &count);
                break;
            case 8:
                displayStatistics(applications, count);
                break;
            case 9:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 9.\n");
                break;
        }
    } while (choice != 9);

    return 0;
}