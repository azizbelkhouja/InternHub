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

// Function prototypes
void encrypt(char *text, int shift);
void decrypt(char *text, int shift);
int isValidDate(char *date);
int isValidCompanyName(char *company);
void saveApplicationToFile(InternshipApplication *application);
void saveAllApplicationsToFile(InternshipApplication *applications, int count);
void loadApplicationsFromFile(InternshipApplication *applications, int *count);
void createBackup();
void restoreFromBackup();
void displayStatistics(InternshipApplication *applications, int count);
int compareDates(const void *a, const void *b);
void addApplication(InternshipApplication *applications, int *count);
void listApplications(InternshipApplication *applications, int count);
void registerUser();
int authenticateUser();
void displayMainMenu(InternshipApplication *applications, int *count);
void clearScreen(); // Function to clear terminal screen

void encrypt(char *text, int shift) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = islower(text[i]) ? 'a' : 'A';
            text[i] = (text[i] - base + shift) % 26 + base;
        }
    }
}

void decrypt(char *text, int shift) {
    encrypt(text, 26 - shift); // Decrypt by shifting backwards
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
    
    strncpy(encryptedCompany, application->company, MAX_STRING_LENGTH);
    strncpy(encryptedJobTitle, application->jobTitle, MAX_STRING_LENGTH);
    strncpy(encryptedMethod, application->applicationMethod, MAX_STRING_LENGTH);
    strncpy(encryptedStatus, application->status, MAX_STRING_LENGTH);
    
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
        
        strncpy(encryptedCompany, applications[i].company, MAX_STRING_LENGTH);
        strncpy(encryptedJobTitle, applications[i].jobTitle, MAX_STRING_LENGTH);
        strncpy(encryptedMethod, applications[i].applicationMethod, MAX_STRING_LENGTH);
        strncpy(encryptedStatus, applications[i].status, MAX_STRING_LENGTH);
        
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
        printf("No existing application data found. Starting fresh.\n");
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
        clearScreen();
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
            strncpy(companies[companyCount], applications[i].company, MAX_STRING_LENGTH);
            companyCounts[companyCount] = 1;
            companyCount++;
        }
    }

    clearScreen();
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

    getchar();  // Wait for user input to close
    clearScreen();
}

int compareDates(const void *a, const void *b) {
    return strcmp(((InternshipApplication *)a)->date, ((InternshipApplication *)b)->date);
}

void addApplication(InternshipApplication *applications, int *count) {
    if (*count >= MAX_APPLICATIONS) {
        printf("Application limit reached.\n");
        return;
    }

    InternshipApplication newApplication;
    printf("Enter application date (dd/mm/yyyy): ");
    scanf("%s", newApplication.date);
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

    printf("Enter application method: ");
    scanf(" %[^\n]", newApplication.applicationMethod);

    printf("Enter application status (Pending/Interviewed/Rejected/Accepted): ");
    scanf(" %[^\n]", newApplication.status);

    applications[*count] = newApplication;
    (*count)++;
    
    saveApplicationToFile(&newApplication);
    printf("Application added successfully.\n");
}

void listApplications(InternshipApplication *applications, int count) {
    if (count == 0) {
        printf("\nNo applications to display.\n");
        getchar();  // Wait for user input to close
        clearScreen();
        return;
    }

    qsort(applications, count, sizeof(InternshipApplication), compareDates);

    clearScreen();
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
    getchar();  // Wait for user input to close
    clearScreen();
}

void registerUser() {
    FILE *file = fopen(USER_DATA_FILE, "a");
    if (file == NULL) {
        printf("Error opening user data file.\n");
        return;
    }

    User newUser;
    printf("Enter username: ");
    scanf("%s", newUser.username);
    printf("Enter password: ");
    scanf("%s", newUser.password);

    fprintf(file, "%s %s\n", newUser.username, newUser.password);

    fclose(file);
    printf("User registered successfully.\n");
}

int authenticateUser() {
    FILE *file = fopen(USER_DATA_FILE, "r");
    if (file == NULL) {
        printf("Error opening user data file.\n");
        return 0;
    }

    char username[MAX_STRING_LENGTH];
    char password[MAX_STRING_LENGTH];
    char storedUsername[MAX_STRING_LENGTH];
    char storedPassword[MAX_STRING_LENGTH];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    while (fscanf(file, "%s %s", storedUsername, storedPassword) != EOF) {
        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void displayMainMenu(InternshipApplication *applications, int *count) {
    int choice;
    do {
        clearScreen();
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
                listApplications(applications, *count);  // Correctly pass the count
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

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    InternshipApplication applications[MAX_APPLICATIONS];
    int count = 0;

    loadApplicationsFromFile(applications, &count);

    printf("Welcome to the Internship Application Tracker\n");
    printf("1. Register\n");
    printf("2. Login\n");
    int option;
    scanf("%d", &option);

    if (option == 1) {
        registerUser();
        
    } else if (option == 2) {
        if (authenticateUser()) {
            printf("Login successful.\n");
            displayMainMenu(applications, &count);
        } else {
            printf("Login failed.\n");
        }
    } else {
        printf("Invalid option.\n");
    }

    saveAllApplicationsToFile(applications, count);

    return 0;
}
