#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 100
#define MAX_NAME 100

typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_TITLE];
    int availableCopies;
} Book;

typedef struct {
    int id;
    char name[MAX_NAME];
    char email[MAX_NAME];
    int issuedBooksCount;
} User;

typedef struct {
    int bookId;
    int userId;
    char issueDate[11]; // YYYY-MM-DD
} IssuedBook;

void addBookToFile(Book book) {
    FILE *file = fopen("books.dat", "ab");
    if (file == NULL) {
        perror("Error opening books file");
        return;
    }
    fwrite(&book, sizeof(Book), 1, file);
    fclose(file);
}

void addUserToFile(User user) {
    FILE *file = fopen("users.dat", "ab");
    if (file == NULL) {
        perror("Error opening users file");
        return;
    }
    fwrite(&user, sizeof(User), 1, file);
    fclose(file);
}

void addIssuedBookToFile(IssuedBook issuedBook) {
    FILE *file = fopen("issued_books.dat", "ab");
    if (file == NULL) {
        perror("Error opening issued books file");
        return;
    }
    fwrite(&issuedBook, sizeof(IssuedBook), 1, file);
    fclose(file);
}

void listBooksFromFile() {
    FILE *file = fopen("books.dat", "rb");
    if (file == NULL) {
        perror("Error opening books file");
        return;
    }
    Book book;
    while (fread(&book, sizeof(Book), 1, file)) {
        printf("ID: %d\n", book.id);
        printf("Title: %s\n", book.title);
        printf("Author: %s\n", book.author);
        printf("Available Copies: %d\n", book.availableCopies);
        printf("-------------------\n");
    }
    fclose(file);
}

void listUsersFromFile() {
    FILE *file = fopen("users.dat", "rb");
    if (file == NULL) {
        perror("Error opening users file");
        return;
    }
    User user;
    while (fread(&user, sizeof(User), 1, file)) {
        printf("ID: %d\n", user.id);
        printf("Name: %s\n", user.name);
        printf("Email: %s\n", user.email);
        printf("Issued Books Count: %d\n", user.issuedBooksCount);
        printf("-------------------\n");
    }
    fclose(file);
}

void addBook() {
    Book book;
    printf("Enter book ID: ");
    while (scanf("%d", &book.id) != 1) {
        printf("Invalid input. Enter a number for book ID: ");
        while (getchar() != '\n'); // Clear input buffer
    }
    printf("Enter book title: ");
    scanf(" %[^\n]", book.title);
    printf("Enter author: ");
    scanf(" %[^\n]", book.author);
    printf("Enter number of available copies: ");
    while (scanf("%d", &book.availableCopies) != 1) {
        printf("Invalid input. Enter a number for available copies: ");
        while (getchar() != '\n'); // Clear input buffer
    }

    addBookToFile(book);
    printf("Book added successfully!\n");
}

void addUser() {
    User user;
    printf("Enter user ID: ");
    while (scanf("%d", &user.id) != 1) {
        printf("Invalid input. Enter a number for user ID: ");
        while (getchar() != '\n'); // Clear input buffer
    }
    printf("Enter user name: ");
    scanf(" %[^\n]", user.name);
    printf("Enter user email: ");
    scanf(" %[^\n]", user.email);
    user.issuedBooksCount = 0;

    addUserToFile(user);
    printf("User added successfully!\n");
}

void issueBook() {
    IssuedBook issuedBook;
    printf("Enter book ID: ");
    while (scanf("%d", &issuedBook.bookId) != 1) {
        printf("Invalid input. Enter a number for book ID: ");
        while (getchar() != '\n'); // Clear input buffer
    }
    printf("Enter user ID: ");
    while (scanf("%d", &issuedBook.userId) != 1) {
        printf("Invalid input. Enter a number for user ID: ");
        while (getchar() != '\n'); // Clear input buffer
    }
    printf("Enter issue date (YYYY-MM-DD): ");
    scanf(" %[^\n]", issuedBook.issueDate);

    addIssuedBookToFile(issuedBook);
    printf("Book issued successfully!\n");
}

void returnBook() {
    int bookId, userId;
    printf("Enter book ID: ");
    while (scanf("%d", &bookId) != 1) {
        printf("Invalid input. Enter a number for book ID: ");
        while (getchar() != '\n'); // Clear input buffer
    }
    printf("Enter user ID: ");
    while (scanf("%d", &userId) != 1) {
        printf("Invalid input. Enter a number for user ID: ");
        while (getchar() != '\n'); // Clear input buffer
    }

    FILE *issuedFile = fopen("issued_books.dat", "rb");
    FILE *tempFile = fopen("temp.dat", "wb");
    if (issuedFile == NULL) {
        perror("Error opening issued books file");
        return;
    }
    if (tempFile == NULL) {
        perror("Error opening temporary file");
        fclose(issuedFile);
        return;
    }

    IssuedBook issuedBook;
    int found = 0;
    while (fread(&issuedBook, sizeof(IssuedBook), 1, issuedFile)) {
        if (issuedBook.bookId == bookId && issuedBook.userId == userId) {
            found = 1;
        } else {
            fwrite(&issuedBook, sizeof(IssuedBook), 1, tempFile);
        }
    }
    fclose(issuedFile);
    fclose(tempFile);

    if (found) {
        // Update the book's available copies
        FILE *booksFile = fopen("books.dat", "r+b");
        if (booksFile == NULL) {
            perror("Error opening books file");
            return;
        }
        Book book;
        int bookFound = 0;
        while (fread(&book, sizeof(Book), 1, booksFile)) {
            if (book.id == bookId) {
                book.availableCopies++;
                fseek(booksFile, -sizeof(Book), SEEK_CUR);
                fwrite(&book, sizeof(Book), 1, booksFile);
                bookFound = 1;
                break;
            }
        }
        fclose(booksFile);

        if (!bookFound) {
            printf("Book with ID %d not found!\n", bookId);
            return;
        }

        // Update the user's issued books count
        FILE *usersFile = fopen("users.dat", "r+b");
        if (usersFile == NULL) {
            perror("Error opening users file");
            return;
        }
        User user;
        int userFound = 0;
        while (fread(&user, sizeof(User), 1, usersFile)) {
            if (user.id == userId) {
                user.issuedBooksCount--;
                fseek(usersFile, -sizeof(User), SEEK_CUR);
                fwrite(&user, sizeof(User), 1, usersFile);
                userFound = 1;
                break;
            }
        }
        fclose(usersFile);

        if (!userFound) {
            printf("User with ID %d not found!\n", userId);
            return;
        }

        // Replace the old issued_books.dat with the updated one
        if (remove("issued_books.dat") != 0) {
            perror("Error deleting old issued books file");
            return;
        }
        if (rename("temp.dat", "issued_books.dat") != 0) {
            perror("Error renaming temporary file");
            return;
        }

        printf("Book returned successfully!\n");
    } else {
        printf("Record not found!\n");
    }
}

void menu() {
    int choice;
    while (1) {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. List Books\n");
        printf("3. Add User\n");
        printf("4. List Users\n");
        printf("5. Issue Book\n");
        printf("6. Return Book\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        while (scanf("%d", &choice) != 1) {
            printf("Invalid input. Enter a number for your choice: ");
            while (getchar() != '\n'); // Clear input buffer
        }

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                listBooksFromFile();
                break;
            case 3:
                addUser();
                break;
            case 4:
                listUsersFromFile();
                break;
            case 5:
                issueBook();
                break;
            case 6:
                returnBook();
                break;
            case 7:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

int main() {
    menu();
    return 0;
}
