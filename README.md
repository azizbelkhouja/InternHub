# Internship Application Tracker - InternHub

[![Run on Repl.it](https://replit.com/badge/github/azizbelkhouja/Internhub)](https://replit.com/github/azizbelkhouja/InternHub)

A console-based application to manage and track internship applications. This project allows users to add, list, and analyze their internship applications, as well as perform user authentication and data management tasks.

## Features

- **User Authentication**: Register and log in with a username and password. Passwords are securely encrypted.
- **Add Application**: Record details of internship applications including date, company, job title, application method, and status.
- **List Applications**: View all recorded applications sorted by date.
- **View Statistics**: Generate statistics including the total number of applications and application status counts.
- **Create Backup**: Create a backup of the application data file.
- **Restore from Backup**: Restore application data from a backup file.

## Requirements

- A C compiler (e.g., GCC)
- Standard C library

## File Structure

- `applications.txt`: Stores encrypted internship application data.
- `backup.txt`: Stores backup of the application data.
- `users.txt`: Stores encrypted user credentials.
- `application_tracker.c`: Main source code file.

## Compilation and Execution

1. **Compile the Program**

   ```sh
   gcc -o internhub internhub.c
   ```

2. **Run the Program**

 ```sh
./internhub
   ```
# Usage
1. Register a New User

- Select option 1 from the main menu to register.
- Enter a username and password when prompted. Passwords are encrypted before being saved.

2. Log In

- Select option 2 from the main menu to log in.
- Enter the registered username and password. Passwords are checked against the encrypted values.

3. Add an Application

- After logging in, select option 1 from the main menu.
- Enter the application details when prompted:
  + Date (format: dd/mm/yyyy)
  + Company Name
  + Job Title
  + Application Method
  + Status (Pending, Interviewed, Rejected, Accepted)

4. List Applications

- Select option 2 from the main menu to list all applications.
- Applications are displayed in reverse chronological order.

5. View Statistics

- Select option 3 from the main menu to view statistics on application statuses and most applied-to companies.

6. Create Backup

- Select option 4 from the main menu to create a backup of the application data file (applications.txt).

7. Restore from Backup

- Select option 5 from the main menu to restore application data from the backup file (backup.txt).

8. Exit

- Select option 6 from the main menu to exit the program.

# **Code Overview**

- Encryption and Decryption: Simple Caesar cipher used for encrypting and decrypting sensitive data.
- File Operations: Read and write application data and user credentials to/from text files.
- Statistics: Computes and displays application statistics.

**Example**

*Registering a User*

 ```sh
Enter username: johndoe
Enter password: mypassword
User registered successfully.
 ```

*Adding an Application*
   
   ```sh
Enter date (dd/mm/yyyy): 23/08/2024
Enter company name: Tech Innovations Ltd
Enter job title: Software Engineer Intern
Enter application method (e.g., online, referral, etc.): Online Application
Enter application status (Pending, Interviewed, Rejected, Accepted): Pending
Application added successfully.
   ```
*Viewing Statistics*

 ```sh

Statistics:
------------------------------------------------------------
Total Applications: 3
Pending: 1
Interviewed: 1
Rejected: 0
Accepted: 1

Most Applied-to Companies:
Tech Innovations Ltd: 1
Company A: 2
------------------------------------------------------------
   ```

# Contributing
Contributions are welcome! Please open an issue or submit a pull request for any improvements or bug fixes.