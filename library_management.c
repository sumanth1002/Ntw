#include <stdio.h>
#include <string.h>

#define MAX 100
#define STUDENT_COUNT 3

typedef struct {
    int id;
    char title[50];
    char author[50];
    char department[20];
    int quantity;
} Book;

typedef struct {
    char username[30];
    char password[30];
    char department[20];
} Student;

typedef struct {
    char username[30];
    int bookId;
} Record;

Book books[MAX] = {
    {101, "Engineering Mathematics", "B S Grewal", "Mathematics", 5},
    {102, "Linear Algebra", "Gilbert Strang", "Mathematics", 3},
    {103, "Engineering Physics", "H K Malik", "Physics", 4},
    {104, "Quantum Physics", "Griffiths", "Physics", 2},
    {105, "Organic Chemistry", "Morrison", "Chemistry", 5},
    {106, "Physical Chemistry", "Atkins", "Chemistry", 3}
};

Student students[STUDENT_COUNT] = {
    {"math1", "1234", "Mathematics"},
    {"physics1", "1234", "Physics"},
    {"chem1", "1234", "Chemistry"}
};

Record records[MAX];
int bookCount = 6;
int recordCount = 0;

void viewBooks(const char department[]) {
    int i, found = 0;

    printf("\n========== BOOK DETAILS ==========\n");
    for (i = 0; i < bookCount; i++) {
        if (strcmp(department, "Admin") == 0 ||
            strcmp(books[i].department, department) == 0) {
            printf("\nBook ID: %d\nTitle: %s\nAuthor: %s\n"
                   "Department: %s\nAvailable Quantity: %d\n",
                   books[i].id, books[i].title, books[i].author,
                   books[i].department, books[i].quantity);
            found = 1;
        }
    }
    if (!found) {
        printf("No books available.\n");
    }
}

void borrowBook(const char username[], const char department[]) {
    int id, i;

    viewBooks(department);
    printf("\nEnter Book ID to borrow: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        return;
    }

    for (i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            /* Enforce department authorization before issuing a book. */
            if (strcmp(books[i].department, department) != 0) {
                printf("Access denied: this book belongs to another department.\n");
                return;
            }
            if (books[i].quantity <= 0) {
                printf("Book is not available.\n");
                return;
            }
            if (recordCount >= MAX) {
                printf("Borrowing record limit reached.\n");
                return;
            }

            books[i].quantity--;
            snprintf(records[recordCount].username,
                     sizeof(records[recordCount].username), "%s", username);
            records[recordCount].bookId = id;
            recordCount++;
            printf("Book borrowed successfully!\n");
            return;
        }
    }
    printf("Invalid Book ID.\n");
}

void addBook(void) {
    Book b;
    int i;

    if (bookCount >= MAX) {
        printf("Library is full.\n");
        return;
    }

    printf("\nEnter Book ID: ");
    if (scanf("%d", &b.id) != 1) {
        printf("Invalid input.\n");
        return;
    }
    for (i = 0; i < bookCount; i++) {
        if (books[i].id == b.id) {
            printf("Book ID already exists.\n");
            return;
        }
    }

    printf("Enter Book Title: ");
    scanf(" %49[^\n]", b.title);
    printf("Enter Author: ");
    scanf(" %49[^\n]", b.author);
    printf("Enter Department (Mathematics/Physics/Chemistry): ");
    scanf("%19s", b.department);

    if (strcmp(b.department, "Mathematics") != 0 &&
        strcmp(b.department, "Physics") != 0 &&
        strcmp(b.department, "Chemistry") != 0) {
        printf("Invalid department.\n");
        return;
    }

    printf("Enter Quantity: ");
    if (scanf("%d", &b.quantity) != 1 || b.quantity < 0) {
        printf("Quantity must be zero or greater.\n");
        return;
    }

    books[bookCount++] = b;
    printf("Book added successfully!\n");
}

void updateBook(void) {
    int id, i, quantity;

    printf("\nEnter Book ID to update: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        return;
    }

    for (i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            printf("Enter new title: ");
            scanf(" %49[^\n]", books[i].title);
            printf("Enter new author: ");
            scanf(" %49[^\n]", books[i].author);
            printf("Enter new quantity: ");
            if (scanf("%d", &quantity) != 1 || quantity < 0) {
                printf("Invalid quantity. Update cancelled.\n");
                return;
            }
            books[i].quantity = quantity;
            printf("Book updated successfully!\n");
            return;
        }
    }
    printf("Book not found.\n");
}

void deleteBook(void) {
    int id, i, j;

    printf("\nEnter Book ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        return;
    }

    for (i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            for (j = i; j < bookCount - 1; j++) {
                books[j] = books[j + 1];
            }
            bookCount--;
            printf("Book deleted successfully!\n");
            return;
        }
    }
    printf("Book not found.\n");
}

void viewRecords(void) {
    int i;

    printf("\n========== BORROWING RECORDS ==========\n");
    if (recordCount == 0) {
        printf("No borrowing records.\n");
        return;
    }
    for (i = 0; i < recordCount; i++) {
        printf("Student: %s | Book ID: %d\n",
               records[i].username, records[i].bookId);
    }
}

void studentMenu(const char username[], const char department[]) {
    int choice;

    do {
        printf("\n===== STUDENT MENU (%s) =====\n", department);
        printf("1. View Department Books\n");
        printf("2. Borrow Book\n");
        printf("3. Logout\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return;
        }

        switch (choice) {
            case 1:
                viewBooks(department);
                break;
            case 2:
                borrowBook(username, department);
                break;
            case 3:
                printf("Logged out successfully.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 3);
}

void adminMenu(void) {
    int choice;

    do {
        printf("\n===== LIBRARY ADMIN MENU =====\n");
        printf("1. View All Books\n");
        printf("2. Add Book\n");
        printf("3. Update Book\n");
        printf("4. Delete Book\n");
        printf("5. View Borrowing Records\n");
        printf("6. Logout\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return;
        }

        switch (choice) {
            case 1:
                viewBooks("Admin");
                break;
            case 2:
                addBook();
                break;
            case 3:
                updateBook();
                break;
            case 4:
                deleteBook();
                break;
            case 5:
                viewRecords();
                break;
            case 6:
                printf("Admin logged out.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 6);
}

void login(void) {
    char username[30], password[30];
    int i;

    printf("\nEnter Username: ");
    scanf("%29s", username);
    printf("Enter Password: ");
    scanf("%29s", password);

    if (strcmp(username, "admin") == 0 &&
        strcmp(password, "admin123") == 0) {
        printf("\nAdmin login successful.\n");
        adminMenu();
        return;
    }

    for (i = 0; i < STUDENT_COUNT; i++) {
        if (strcmp(username, students[i].username) == 0 &&
            strcmp(password, students[i].password) == 0) {
            printf("\nStudent login successful. Department: %s\n",
                   students[i].department);
            studentMenu(username, students[i].department);
            return;
        }
    }

    printf("Invalid username or password.\n");
}

int main(void) {
    int choice;

    do {
        printf("\n====================================\n");
        printf(" DEPARTMENT LIBRARY MANAGEMENT SYSTEM\n");
        printf("====================================\n");
        printf("1. Login\n");
        printf("2. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return 1;
        }

        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                printf("Thank you for using the Library System!\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 2);

    return 0;
}
