#include <stdio.h>
#include <string.h>

struct Book {
    int id;
    int copies[10];         // 1 = Available, 0 = Borrowed
    char borrower[10][30];  // Stores name of user who borrowed each copy
    char title[50], cat[15];
} b[50] = {
    {1, {1,1,1,1,1,1,1,1,1,1}, {""}, "Calculus", "Math"},
    {2, {1,1,1,1,1,1,1,1,1,1}, {""}, "Linear_Algebra", "Math"},
    {3, {1,1,1,1,1,1,1,1,1,1}, {""}, "Concepts_of_Physics", "Physics"},
    {4, {1,1,1,1,1,1,1,1,1,1}, {""}, "Feynman_Lectures", "Physics"},
    {5, {1,1,1,1,1,1,1,1,1,1}, {""}, "Organic_Chemistry", "Chemistry"},
    {6, {1,1,1,1,1,1,1,1,1,1}, {""}, "Physical_Chemistry", "Chemistry"}
};
int count = 6;
int accessCount[3] = {0}; // 0: Math, 1: Physics, 2: Chemistry

int getCatIndex(const char *cat) {
    if (!strcasecmp(cat, "Math")) return 0;
    if (!strcasecmp(cat, "Physics")) return 1;
    if (!strcasecmp(cat, "Chemistry")) return 2;
    return -1;
}

int getAvailCount(int idx) {
    int c = 0;
    for (int i = 0; i < 10; i++) c += b[idx].copies[i];
    return c;
}

void showBooks(const char *cat) {
    printf("\nID\tSubject\t\tStatus\t\tCopies\tTitle\n");
    for (int i = 0; i < count; i++) {
        if (!strcmp(cat, "ALL") || !strcasecmp(b[i].cat, cat)) {
            int av = getAvailCount(i);
            printf("%d\t%-10s\t%-13s\t%d/10\t%s\n",
                   b[i].id, b[i].cat, av > 0 ? "Available" : "Not Available", av, b[i].title);
        }
    }
}

void showCopyAndBorrowerRecords() {
    printf("\n--- All 10-Copy & Borrower Records ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID %d (%s - %s) [%s]:\n  ", b[i].id, b[i].title, b[i].cat,
               getAvailCount(i) > 0 ? "Available" : "Not Available");
        for (int c = 0; c < 10; c++) {
            if (b[i].copies[c])
                printf("C%d:Avail ", c + 1);
            else
                printf("C%d:Borrowed(%s) ", c + 1, b[i].borrower[c]);
        }
        printf("\n");
    }
}

int main() {
    int ch, subCh, id, idx;
    char user[30], pass[20], sub[15];

    while (1) {
        printf("\n1.Admin  2.User  3.Exit: ");
        scanf("%d", &ch);

        if (ch == 1) {
            printf("ID: "); scanf("%19s", user);
            printf("Pass: "); scanf("%19s", pass);
            if (strcmp(user, "admin") || strcmp(pass, "admin@123")) {
                printf("Invalid credentials!\n");
                continue;
            }
            while (1) {
                printf("\n[Admin] 1.All Books  2.Add Book  3.Access Stats  4.Borrower & Copy Records  5.Logout: ");
                scanf("%d", &subCh);
                if (subCh == 1) showBooks("ALL");
                else if (subCh == 2 && count < 50) {
                    b[count].id = count + 1;
                    for (int c = 0; c < 10; c++) {
                        b[count].copies[c] = 1;
                        b[count].borrower[c][0] = '\0';
                    }
                    printf("Title (use _ for spaces): "); scanf("%49s", b[count].title);
                    printf("Category (Math/Physics/Chemistry): "); scanf("%14s", b[count].cat);
                    count++;
                } else if (subCh == 3) {
                    printf("\n--- Category Access Statistics ---\n");
                    printf("Math: %d users | Physics: %d users | Chemistry: %d users\n",
                           accessCount[0], accessCount[1], accessCount[2]);
                } else if (subCh == 4) showCopyAndBorrowerRecords();
                else break;
            }
        } else if (ch == 2) {
            printf("Enter Your Name: ");
            scanf("%29s", user);
            printf("Welcome, %s! Enter Subject (Math/Physics/Chemistry): ", user);
            scanf("%14s", sub);
            idx = getCatIndex(sub);
            if (idx == -1) {
                printf("Only Math, Physics, or Chemistry allowed!\n");
                continue;
            }
            accessCount[idx]++;

            while (1) {
                printf("\n[%s | %s] 1.View  2.Borrow  3.Back: ", user, sub);
                scanf("%d", &subCh);
                if (subCh == 1) showBooks(sub);
                else if (subCh == 2) {
                    showBooks(sub);
                    printf("Book ID: "); scanf("%d", &id);
                    int found = 0;
                    for (int i = 0; i < count; i++) {
                        if (b[i].id == id && !strcasecmp(b[i].cat, sub)) {
                            found = 1;
                            int borrowedCopy = -1;
                            for (int c = 0; c < 10; c++) {
                                if (b[i].copies[c] == 1) {
                                    b[i].copies[c] = 0;
                                    strcpy(b[i].borrower[c], user);
                                    borrowedCopy = c + 1;
                                    break;
                                }
                            }
                            if (borrowedCopy != -1)
                                printf("%s borrowed '%s' (Copy #%d)! Remaining: %d/10\n",
                                       user, b[i].title, borrowedCopy, getAvailCount(i));
                            else
                                printf("Books are not available! All 10 copies of '%s' are borrowed.\n", b[i].title);
                        }
                    }
                    if (!found) printf("Book ID not found in %s!\n", sub);
                } else break;
            }
        } else break;
    }
    return 0;
}
