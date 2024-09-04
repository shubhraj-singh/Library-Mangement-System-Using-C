# Library Management System in C

## Overview

The Library Management System (LMS) is a simple console-based application written in C. It allows users to manage books and users, issue and return books, and view records. The system uses binary files to store data about books, users, and issued books.

## Features

- Add new books and users
- List all books and users
- Issue books to users
- Return books to the library
- Basic error handling for file operations and user inputs

## Files

- `main.c`: Main source file containing the implementation of the library management system.
- `books.dat`: Binary file to store book records.
- `users.dat`: Binary file to store user records.
- `issued_books.dat`: Binary file to store issued book records.

## Build and Run

1. **Build the Program**

   To compile the `main.c` file, use a C compiler like `gcc`. Open a terminal and run:

   ```bash
   gcc -o main main.c
