# Internship Application Tracker

[![Run on Repl.it](https://replit.com/badge/github/azizbelkhouja/InternHub)](https://replit.com/github/azizbelkhouja/InternHub)


This C program is a simple console-based application designed to help you track your internship applications. You can add, view, update, and delete your applications, as well as keep track of the application status. The applications are stored in a text file, making it easy to save and retrieve data.

## Features

- **Add New Application**: Record the details of your internship applications, including the date of application, company name, job title, application method, and status (default is "Pending").
- **Display All Applications**: View all your applications sorted by date, from the latest to the earliest.
- **Update Application Status**: Update the status of an existing application (Accepted, Rejected, Pending).
- **Delete Application(s)**: Delete a specific application or all applications from the record, with confirmation prompts.
- **File Storage**: All applications are stored in a text file (`applications.txt`), ensuring persistence across sessions.

## Usage

### Adding an Application
1. Choose the "Add New Application" option from the menu.
2. Enter the required details:
   - **Date of Application**: Format should be `dd/mm/yyyy`. The program validates the date to ensure it's within the range from 01/01/2000 to 31/12/2025.
   - **Company Name**: Name of the company you're applying to.
   - **Job Title**: The title of the job you're applying for.
   - **Application Method**: Choose from LinkedIn, Phone, Cold Mail, Indeed, or Almalaurea.
   - **Status**: Automatically set to "Pending."

### Displaying All Applications
1. Choose the "Display All Applications" option from the menu.
2. The program will list all applications sorted by the application date, showing the latest applications first.

### Updating an Application Status
1. Choose the "Update Application Status" option from the menu.
2. Enter the company name of the application you want to update.
3. Select the new status (Accepted, Rejected, Pending).
4. The program will update the status in the file.

### Deleting Applications
1. Choose the "Delete Application(s)" option from the menu.
2. Select whether to delete one specific application or all applications.
   - **Single Application**: Enter the company name and job title. The program will prompt for confirmation before deletion.
   - **All Applications**: The program will prompt for confirmation before deleting all records.

### Exiting the Program
- Choose the "Exit" option from the menu to safely close the program.

## File Storage

All application data is stored in `applications.txt`, which is created in the same directory as the program. This file is used to load the data when the program starts and save any updates during the session.
