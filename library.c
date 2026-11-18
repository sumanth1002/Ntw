/*
 * Library Management System
 * Compile: gcc library.c -o library
 * Run:     ./library
 *
 * Demo logins:
 * Admin:   admin       / admin123
 * Maths:   maths1      / maths123
 * Physics: physics1    / physics123
 * Chemistry: chem1     / chem123
 *
 * Data is stored in books.txt and loans.txt in the current directory.
 * This is a training/demo program. Change the demo passwords before use.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOK_FILE "books.txt"
#define LOAN_FILE "loans.txt"
#define MAX_BOOKS 500
#define MAX_LOANS 1000
#define TITLE_LEN 100
#define USER_LEN 40

typedef struct {
    int id;
    char title[TITLE_LEN];
    char subject[20];
    int total;
    int available;
} Book;

typedef struct {
    int book_id;
    char username[USER_LEN];
} Loan;

Book books[MAX_BOOKS];
Loan loans[MAX_LOANS];
int book_count = 0, loan_count = 0;

void read_line(const char *prompt, char *out, size_t size) {
    printf("%s", prompt);
    if (fgets(out, (int)size, stdin)) {
        out[strcspn(out, "\n")] = '\0';
    } else {
        out[0] = '\0';
    }
}

int read_int(const char *prompt) {
    char buf[40], *end;
    long value;
    while (1) {
        read_line(prompt, buf, sizeof(buf));
        value = strtol(buf, &end, 10);
        if (end != buf && *end == '\0') return (int)value;
        printf("Please enter a valid number.\n");
    }
}

void save_books(void) {
    FILE *fp = fopen(BOOK_FILE, "w");
    if (!fp) { perror("Could not save books"); return; }
    for (int i = 0; i < book_count; i++)
        fprintf(fp, "%d|%s|%s|%d|%d\n", books[i].id, books[i].title,
                books[i].subject, books[i].total, books[i].available);
    fclose(fp);
}

void save_loans(void) {
    FILE *fp = fopen(LOAN_FILE, "w");
    if (!fp) { perror("Could not save loans"); return; }
    for (int i = 0; i < loan_count; i++)
        fprintf(fp, "%d|%s\n", loans[i].book_id, loans[i].username);
    fclose(fp);
}

void load_data(void) {
    FILE *fp = fopen(BOOK_FILE, "r");
    if (fp) {
        char line[256];
        while (fgets(line, sizeof(line), fp) && book_count < MAX_BOOKS) {
            Book b;
            if (sscanf(line, "%d|%99[^|]|%19[^|]|%d|%d",
                       &b.id, b.title, b.subject, &b.total, &b.available) == 5)
                books[book_count++] = b;
        }
        fclose(fp);
    } else {
        Book starter[] = {
            {101, "Engineering Mathematics", "Maths", 5, 5},
            {102, "Linear Algebra", "Maths", 3, 3},
            {201, "Fundamentals of Physics", "Physics", 4, 4},
            {202, "Optics", "Physics", 2, 2},
            {301, "Organic Chemistry", "Chemistry", 4, 4},
            {302, "Physical Chemistry", "Chemistry", 3, 3}
        };
        book_count = (int)(sizeof(starter) / sizeof(starter[0]));
        memcpy(books, starter, sizeof(starter));
        save_books();
    }

    fp = fopen(LOAN_FILE, "r");
    if (fp) {
        char line[100], username[USER_LEN];
        int id;
        while (fgets(line, sizeof(line), fp) && loan_count < MAX_LOANS) {
            if (sscanf(line, "%d|%39[^\n]", &id, username) == 2) {
                loans[loan_count].book_id = id;
                strcpy(loans[loan_count].username, username);
                loan_count++;
            }
        }
        fclose(fp);
    }
}

const char *subject_for_user(const char *username, const char *password) {
    if (!strcmp(username, "maths1") && !strcmp(password, "maths123")) return "Maths";
    if (!strcmp(username, "physics1") && !strcmp(password, "physics123")) return "Physics";
    if (!strcmp(username, "chem1") && !strcmp(password, "chem123")) return "Chemistry";
    return NULL;
}

int find_book(int id) {
    for (int i = 0; i < book_count; i++)
        if (books[i].id == id) return i;
    return -1;
}

int user_has_book(const char *username, int book_id) {
    for (int i = 0; i < loan_count; i++)
        if (loans[i].book_id == book_id &&
            !strcmp(loans[i].username, username)) return 1;
    return 0;
}

void list_books(const char *allowed_subject) {
    int found = 0;
    printf("\n%-6s %-32s %-12s %-8s %-10s\n",
           "ID", "TITLE", "SUBJECT", "TOTAL", "AVAILABLE");
    printf("--------------------------------------------------------------------------\n");
    for (int i = 0; i < book_count; i++) {
        if (!allowed_subject || !strcmp(books[i].subject, allowed_subject)) {
            printf("%-6d %-32.32s %-12s %-8d %-10d\n",
                   books[i].id, books[i].title, books[i].subject,
                   books[i].total, books[i].available);
            found = 1;
        }
    }
    if (!found) printf("No books found for this subject.\n");
}

void add_book(void) {
    if (book_count >= MAX_BOOKS) { printf("Book storage is full.\n"); return; }
    Book b;
    b.id = read_int("Enter new book ID: ");
    if (find_book(b.id) != -1) { printf("That book ID already exists.\n"); return; }

    read_line("Enter book title (do not use |): ", b.title, sizeof(b.title));
    printf("Subject: 1. Maths  2. Physics  3. Chemistry\n");
    int choice = read_int("Choose subject: ");
    if (choice == 1) strcpy(b.subject, "Maths");
    else if (choice == 2) strcpy(b.subject, "Physics");
    else if (choice == 3) strcpy(b.subject, "Chemistry");
    else { printf("Invalid subject.\n"); return; }

    b.total = read_int("Enter number of copies: ");
    if (b.total <= 0) { printf("Copies must be greater than zero.\n"); return; }
    b.available = b.total;
    books[book_count++] = b;
    save_books();
    printf("Book added successfully.\n");
}

void issue_book(const char *username, const char *allowed_subject) {
    int id = read_int("Enter book ID to issue: ");
    int index = find_book(id);
    if (index == -1) { printf("Book not found.\n"); return; }

    if (allowed_subject && strcmp(books[index].subject, allowed_subject)) {
        printf("Access denied: you can issue only %s books.\n", allowed_subject);
        return;
    }
    if (books[index].available <= 0) { printf("No copies are currently available.\n"); return; }
    if (user_has_book(username, id)) { printf("You have already issued this book.\n"); return; }
    if (loan_count >= MAX_LOANS) { printf("Loan storage is full.\n"); return; }

    books[index].available--;
    loans[loan_count].book_id = id;
    strcpy(loans[loan_count].username, username);
    loan_count++;
    save_books();
    save_loans();
    printf("Book issued successfully to %s.\n", username);
}

void return_book(const char *username) {
    int id = read_int("Enter book ID to return: ");
    int loan_index = -1;
    for (int i = 0; i < loan_count; i++) {
        if (loans[i].book_id == id && !strcmp(loans[i].username, username)) {
            loan_index = i;
            break;
        }
    }
    if (loan_index == -1) {
        printf("No matching issue record found for your account.\n");
        return;
    }

    int book_index = find_book(id);
    if (book_index != -1 && books[book_index].available < books[book_index].total)
        books[book_index].available++;

    for (int i = loan_index; i < loan_count - 1; i++)
        loans[i] = loans[i + 1];
    loan_count--;
    save_books();
    save_loans();
    printf("Book returned successfully.\n");
}

void show_my_books(const char *username) {
    int found = 0;
    printf("\nBooks currently issued to %s:\n", username);
    for (int i = 0; i < loan_count; i++) {
        if (!strcmp(loans[i].username, username)) {
            int index = find_book(loans[i].book_id);
            if (index != -1) {
                printf("ID: %d | %s | %s\n", books[index].id,
                       books[index].title, books[index].subject);
                found = 1;
            }
        }
    }
    if (!found) printf("No books currently issued.\n");
}

void admin_menu(void) {
    int choice;
    do {
        printf("\n========== ADMIN MENU ==========\n");
        printf("1. View all books\n2. Add a book\n3. Issue a book to a student\n");
        printf("4. Return a student's book\n5. Exit admin menu\n");
        choice = read_int("Enter choice: ");
        if (choice == 1) list_books(NULL);
        else if (choice == 2) add_book();
        else if (choice == 3) {
            char username[USER_LEN];
            read_line("Enter student username (maths1/physics1/chem1): ",
                      username, sizeof(username));
            if (strcmp(username, "maths1") && strcmp(username, "physics1") &&
                strcmp(username, "chem1")) {
                printf("Unknown demo student username.\n");
            } else {
                const char *subject = !strcmp(username, "maths1") ? "Maths" :
                                      !strcmp(username, "physics1") ? "Physics" : "Chemistry";
                issue_book(username, subject);
            }
        }
        else if (choice == 4) {
            char username[USER_LEN];
            read_line("Enter student username: ", username, sizeof(username));
            return_book(username);
        }
        else if (choice != 5) printf("Invalid choice.\n");
    } while (choice != 5);
}

void student_menu(const char *username, const char *subject) {
    int choice;
    do {
        printf("\n======= %s STUDENT MENU =======\n", subject);
        printf("1. View %s books\n2. Issue a book\n", subject);
        printf("3. Return my book\n4. View my issued books\n5. Logout\n");
        choice = read_int("Enter choice: ");
        if (choice == 1) list_books(subject);
        else if (choice == 2) issue_book(username, subject);
        else if (choice == 3) return_book(username);
        else if (choice == 4) show_my_books(username);
        else if (choice != 5) printf("Invalid choice.\n");
    } while (choice != 5);
}

int main(void) {
    char username[USER_LEN], password[USER_LEN];
    load_data();

    printf("====================================\n");
    printf("       LIBRARY MANAGEMENT SYSTEM\n");
    printf("====================================\n");
    read_line("Username: ", username, sizeof(username));
    read_line("Password: ", password, sizeof(password));

    if (!strcmp(username, "admin") && !strcmp(password, "admin123")) {
        printf("\nAdmin login successful.\n");
        admin_menu();
    } else {
        const char *subject = subject_for_user(username, password);
        if (subject) {
            printf("\nStudent login successful. Access granted to %s books only.\n", subject);
            student_menu(username, subject);
        } else {
            printf("\nInvalid username or password. Access denied.\n");
        }
    }

    printf("\nThank you for using the Library Management System.\n");
    return 0;
}
