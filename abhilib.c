#include <stdio.h>
#include <string.h>

struct Book
{
    int id;
    char name[50], subject[20];
};

struct Book b[50];
int n = 0;

void save()
{
    FILE *fp = fopen("books.txt", "w");
    int i;
    if (fp == NULL) return;
    for (i = 0; i < n; i++)
        fprintf(fp, "%d|%s|%s\n", b[i].id, b[i].name, b[i].subject);
    fclose(fp);
}

void load()
{
    FILE *fp = fopen("books.txt", "r");
    if (fp != NULL)
    {
        while (n < 50 && fscanf(fp, "%d|%49[^|]|%19[^\n]\n",
               &b[n].id, b[n].name, b[n].subject) == 3)
            n++;
        fclose(fp);
    }
    if (n == 0)
    {
        b[0] = (struct Book){101, "Engineering Mathematics", "Maths"};
        b[1] = (struct Book){102, "Calculus", "Maths"};
        b[2] = (struct Book){201, "Engineering Physics", "Physics"};
        b[3] = (struct Book){202, "Thermodynamics", "Physics"};
        b[4] = (struct Book){301, "Organic Chemistry", "Chemistry"};
        b[5] = (struct Book){302, "Physical Chemistry", "Chemistry"};
        n = 6;
        save();
    }
}

void show(char subject[])
{
    int i, count, choice, index[50];
    while (1)
    {
        count = 0;
        printf("\n--- %s Books ---\n", subject);
        for (i = 0; i < n; i++)
            if (strcmp(b[i].subject, subject) == 0)
            {
                index[count] = i;
                printf("%d. %s\n", count + 1, b[i].name);
                count++;
            }
        if (count == 0) printf("No books available.\n");
        printf("0. Back to Subjects\nEnter choice: ");
        scanf("%d", &choice);
        if (choice == 0) return;
        if (choice > 0 && choice <= count)
            printf("Selected: %s\n", b[index[choice - 1]].name);
        else printf("Invalid choice!\n");
    }
}

void user()
{
    int choice;
    do
    {
        printf("\n--- Select Subject ---\n");
        printf("1. Maths\n2. Physics\n3. Chemistry\n0. Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1: show("Maths"); break;
            case 2: show("Physics"); break;
            case 3: show("Chemistry"); break;
            case 0: break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);
}

void view_all()
{
    int i;
    if (n == 0) printf("No books available.\n");
    for (i = 0; i < n; i++)
        printf("%d. %s (%s)\n", b[i].id, b[i].name, b[i].subject);
}

void add()
{
    struct Book temp;
    int i, choice;
    if (n == 50) { printf("Book list is full.\n"); return; }
    printf("Book ID: ");
    scanf("%d", &temp.id);
    for (i = 0; i < n; i++)
        if (b[i].id == temp.id)
        { printf("Book ID already exists!\n"); return; }
    printf("Book name: ");
    scanf(" %49[^\n]", temp.name);
    printf("1. Maths  2. Physics  3. Chemistry\nSubject: ");
    scanf("%d", &choice);
    switch (choice)
    {
        case 1: strcpy(temp.subject, "Maths"); break;
        case 2: strcpy(temp.subject, "Physics"); break;
        case 3: strcpy(temp.subject, "Chemistry"); break;
        default: printf("Invalid subject!\n"); return;
    }
    b[n++] = temp;
    save();
    printf("Book added.\n");
}

void delete_book()
{
    int id, i, j;
    printf("Book ID to delete: ");
    scanf("%d", &id);
    for (i = 0; i < n; i++)
        if (b[i].id == id) break;
    if (i == n) { printf("Book not found!\n"); return; }
    for (j = i; j < n - 1; j++) b[j] = b[j + 1];
    n--;
    save();
    printf("Book deleted.\n");
}

void admin()
{
    char user[20], pass[20];
    int choice;
    printf("Username: ");
    scanf("%19s", user);
    printf("Password: ");
    scanf("%19s", pass);
    if (strcmp(user, "admin") || strcmp(pass, "admin123"))
    { printf("Invalid login!\n"); return; }

    do
    {
        printf("\n--- Admin Menu ---\n");
        printf("1. View All Books\n2. Add Book\n3. Delete Book\n0. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1: view_all(); break;
            case 2: add(); break;
            case 3: delete_book(); break;
            case 0: printf("Logged out.\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);
}

int main()
{
    int choice;
    load();
    do
    {
        printf("\n=== LIBRARY MANAGEMENT SYSTEM ===\n");
        printf("1. User\n2. Admin\n0. Exit\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1: user(); break;
            case 2: admin(); break;
            case 0: printf("Thank you!\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);
    return 0;
}
