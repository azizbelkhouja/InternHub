#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_APPLICATIONS 100
#define MAX_STRING_LENGTH 100
#define FILENAME "applications.txt"
#define BACKUP_FILENAME "backup.txt"
#define SHIFT 3
#define USER_DATA_FILE "users.txt" // File to store user data

typedef struct {
    char date[MAX_STRING_LENGTH];
    char company[MAX_STRING_LENGTH];
    char jobTitle[MAX_STRING_LENGTH];
    char applicationMethod[MAX_STRING_LENGTH];
    char status[MAX_STRING_LENGTH];
} InternshipApplication;

typedef struct {
    char username[MAX_STRING_LENGTH];
    char password[MAX_STRING_LENGTH];
} User;

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
    InternshipApplication newApplication;
    
    printf("Enter date (dd/mm/yyyy): ");
    scanf(" %s", newApplication.date);
    if (!isValidDate(newApplication.date)) {
        printf("Invalid date format.\n");
        return;
    }
    
    printf("Enter company name: ");
    scanf(" %[^\n]", newApplication.company);
    if (!isValidCompanyName(newApplication.company)) {
        printf("Invalid company name.\n");
        return;
    }
    
    printf("Enter job title: ");
    scanf(" %[^\n]", newApplication.jobTitle);
    
    printf("Enter application method (e.g., online, referral, etc.): ");
    scanf(" %[^\n]", newApplication.applicationMethod);
    
    printf("Enter application status (Pending, Interviewed, Rejected, Accepted): ");
    scanf(" %[^\n]", newApplication.status);

    applications[*count] = newApplication;
    (*count)++;
    saveApplicationToFile(&newApplication);
    
    printf("Application added successfully.\n");
}

void listApplications(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("\nNo applications to display.\n");
        getchar();
        return;
    }

    qsort(applications, count, sizeof(InternshipApplication), compareDates);

    
    printf("\nApplications:\n");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("Date: %s\n", applications[i].date);
        printf("Company: %s\n", applications[i].company);
        printf("Job Title: %s\n", applications[i].jobTitle);
        printf("Application Method: %s\n", applications[i].applicationMethod);
        printf("Status: %s\n", applications[i].status);
        printf("------------------------------------------------------------\n");
    }
    getchar();
}

void registerUser() {
    FILE *file = fopen(USER_DATA_FILE, "a");
    if (file == NULL) {
        printf("Error opening file for user registration.\n");
        return;
    }
    
    User newUser;
    printf("Enter username: ");
    scanf(" %s", newUser.username);
    printf("Enter password: ");
    scanf(" %s", newUser.password);

    encrypt(newUser.password, SHIFT);
    
    fprintf(file, "%s|%s\n", newUser.username, newUser.password);
    fclose(file);
    
    printf("User registered successfully.\n");
}

int authenticateUser() {
    FILE *file = fopen(USER_DATA_FILE, "r");
    if (file == NULL) {
        printf("Error opening file for authentication.\n");
        return 0;
    }

    char username[MAX_STRING_LENGTH];
    char password[MAX_STRING_LENGTH];
    char storedUsername[MAX_STRING_LENGTH];
    char storedPassword[MAX_STRING_LENGTH];
    
    printf("Enter username: ");
    scanf(" %s", username);
    printf("Enter password: ");
    scanf(" %s", password);
    
    encrypt(password, SHIFT);
    
    while (fscanf(file, "%[^|]|%[^\n]\n", storedUsername, storedPassword) != EOF) {
        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    printf("Invalid username or password.\n");
    return 0;
}

void displayMainMenu(InternshipApplication *applications, int *count) {
    int choice;
    do {
        
        printf("Application Tracker\n");
        printf("1. Add Application\n");
        printf("2. List Applications\n");
        printf("3. View Statistics\n");
        printf("4. Create Backup\n");
        printf("5. Restore from Backup\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addApplication(applications, count);
                break;
            case 2:
                listApplications(applications, *count);
                break;
            case 3:
                displayStatistics(applications, *count);
                break;
            case 4:
                createBackup();
                break;
            case 5:
                restoreFromBackup();
                break;
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
}

int main() {
    InternshipApplication applications[MAX_APPLICATIONS];
    int count = 0;

    int authenticated = 0;
    while (!authenticated) {
        
        printf("1. Register\n");
        printf("2. Login\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                authenticated = authenticateUser();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    loadApplicationsFromFile(applications, &count);
    displayMainMenu(applications, &count);
    saveAllApplicationsToFile(applications, count);

    return 0;
}
