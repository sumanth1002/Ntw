#include <stdio.h>
#include <string.h>

struct Book {
    int id, qty;
    char name[50], subject[20];
};
struct Issue {
    int id;
    char user[20];
};

struct Book b[50];
struct Issue issue[100];
int n = 0, ni = 0;

void saveBooks() {
    FILE *fp = fopen("books.txt", "w");
    int i;
    if (fp == NULL) return;
    for (i = 0; i < n; i++)
        fprintf(fp, "%d|%s|%s|%d\n", b[i].id, b[i].name, b[i].subject, b[i].qty);
    fclose(fp);
}

void saveIssues() {
    FILE *fp = fopen("issued.txt", "w");
    int i;
    if (fp == NULL) return;
    for (i = 0; i < ni; i++)
        fprintf(fp, "%d|%s\n", issue[i].id, issue[i].user);
    fclose(fp);
}

void loadData() {
    FILE *fp = fopen("books.txt", "r");
    if (fp != NULL) {
        while (n < 50 && fscanf(fp, "%d|%49[^|]|%19[^|]|%d\n",
               &b[n].id, b[n].name, b[n].subject, &b[n].qty) == 4) n++;
        fclose(fp);
    }
    if (n == 0) {
        b[0].id=101; strcpy(b[0].name,"Engineering Maths");
        strcpy(b[0].subject,"Maths"); b[0].qty=5;
        b[1].id=201; strcpy(b[1].name,"Basic Physics");
        strcpy(b[1].subject,"Physics"); b[1].qty=4;
        b[2].id=301; strcpy(b[2].name,"Organic Chemistry");
        strcpy(b[2].subject,"Chemistry"); b[2].qty=3;
        n=3; saveBooks();
    }
    fp = fopen("issued.txt", "r");
    if (fp != NULL) {
        while (ni < 100 && fscanf(fp, "%d|%19[^\n]\n",
               &issue[ni].id, issue[ni].user) == 2) ni++;
        fclose(fp);
    }
}

int findBook(int id) {
    int i;
    for (i=0; i<n; i++) if (b[i].id == id) return i;
    return -1;
}

int hasIssued(char user[], int id) {
    int i;
    for (i=0; i<ni; i++)
        if (issue[i].id==id && strcmp(issue[i].user,user)==0) return 1;
    return 0;
}

void viewBooks(char subject[]) {
    int i, found=0;
    printf("\nID\tBook Name\t\tSubject\t\tAvailable\n");
    for (i=0; i<n; i++) {
        if (strcmp(subject,"All")==0 || strcmp(subject,b[i].subject)==0) {
            printf("%d\t%-20s\t%-12s\t%d\n",b[i].id,b[i].name,b[i].subject,b[i].qty);
            found=1;
        }
    }
    if (!found) printf("No books found.\n");
}

void searchBook(char subject[]) {
    int id, p;
    printf("Enter book ID: "); scanf("%d",&id);
    p=findBook(id);
    if (p==-1) printf("Book not found.\n");
    else if (strcmp(subject,"All")!=0 && strcmp(subject,b[p].subject)!=0)
        printf("Access denied. This book belongs to another subject.\n");
    else printf("%s | %s | Available: %d\n",b[p].name,b[p].subject,b[p].qty);
}

void addBook() {
    int c;
    if (n==50) { printf("Book list is full.\n"); return; }
    printf("Book ID: "); scanf("%d",&b[n].id);
    if (findBook(b[n].id)!=-1) { printf("ID already exists.\n"); return; }
    printf("Book name: "); scanf(" %49[^\n]",b[n].name);
    printf("1. Maths  2. Physics  3. Chemistry\nSubject: "); scanf("%d",&c);
    if (c==1) strcpy(b[n].subject,"Maths");
    else if (c==2) strcpy(b[n].subject,"Physics");
    else if (c==3) strcpy(b[n].subject,"Chemistry");
    else { printf("Invalid subject.\n"); return; }
    printf("Quantity: "); scanf("%d",&b[n].qty);
    if (b[n].qty<=0) { printf("Invalid quantity.\n"); return; }
    n++; saveBooks(); printf("Book added.\n");
}

void deleteBook() {
    int id,i,j;
    printf("Book ID to delete: "); scanf("%d",&id);
    for (i=0; i<n; i++) if (b[i].id==id) break;
    if (i==n) { printf("Book not found.\n"); return; }
    for (j=0; j<ni; j++)
        if (issue[j].id==id) { printf("Book is currently issued.\n"); return; }
    for (j=i; j<n-1; j++) b[j]=b[j+1];
    n--; saveBooks(); printf("Book deleted.\n");
}

void issueBook(char user[], char subject[]) {
    int id,p;
    printf("Book ID to issue: "); scanf("%d",&id);
    p=findBook(id);
    if (p==-1) printf("Book not found.\n");
    else if (strcmp(subject,"All")!=0 && strcmp(subject,b[p].subject)!=0)
        printf("Access denied. You can issue only your subject books.\n");
    else if (b[p].qty==0) printf("Book not available.\n");
    else if (hasIssued(user,id)) printf("You already issued this book.\n");
    else if (ni==100) printf("Issue list is full.\n");
    else {
        b[p].qty--; issue[ni].id=id; strcpy(issue[ni].user,user); ni++;
        saveBooks(); saveIssues(); printf("Book issued.\n");
    }
}

void returnBook(char user[], char subject[]) {
    int id,p,i,j;
    printf("Book ID to return: "); scanf("%d",&id);
    p=findBook(id);
    if (p==-1) { printf("Book not found.\n"); return; }
    if (strcmp(subject,"All")!=0 && strcmp(subject,b[p].subject)!=0) {
        printf("Access denied.\n"); return;
    }
    for (i=0; i<ni; i++)
        if (issue[i].id==id && strcmp(issue[i].user,user)==0) break;
    if (i==ni) { printf("This book was not issued to you.\n"); return; }
    for (j=i; j<ni-1; j++) issue[j]=issue[j+1];
    ni--; b[p].qty++; saveBooks(); saveIssues();
    printf("Book returned.\n");
}

void myBooks(char user[]) {
    int i,p,found=0;
    printf("\nYour issued books:\n");
    for (i=0; i<ni; i++) {
        if (strcmp(issue[i].user,user)==0) {
            p=findBook(issue[i].id);
            if (p!=-1) { printf("%d - %s\n",b[p].id,b[p].name); found=1; }
        }
    }
    if (!found) printf("No books issued.\n");
}

void admin() {
    char user[20],pass[20];
    int c;
    printf("Username: "); scanf("%19s",user);
    printf("Password: "); scanf("%19s",pass);
    if (strcmp(user,"admin")!=0 || strcmp(pass,"admin123")!=0) {
        printf("Invalid login.\n"); return;
    }
    do {
        printf("\n--- ADMIN MENU ---\n1. View all books\n2. Add book\n3. Delete book\n4. Search book\n0. Logout\nChoice: ");
        scanf("%d",&c);
        switch(c) {
            case 1: viewBooks("All"); break;
            case 2: addBook(); break;
            case 3: deleteBook(); break;
            case 4: searchBook("All"); break;
            case 0: printf("Logged out.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while(c!=0);
}

void student() {
    char user[20],pass[20],subject[20];
    int c;
    printf("Username: "); scanf("%19s",user);
    printf("Password: "); scanf("%19s",pass);
    if (strcmp(user,"maths")==0 && strcmp(pass,"maths123")==0)
        strcpy(subject,"Maths");
    else if (strcmp(user,"physics")==0 && strcmp(pass,"physics123")==0)
        strcpy(subject,"Physics");
    else if (strcmp(user,"chemistry")==0 && strcmp(pass,"chem123")==0)
        strcpy(subject,"Chemistry");
    else { printf("Invalid login.\n"); return; }

    do {
        printf("\n--- %s STUDENT MENU ---\n1. View books\n2. Search book\n3. Issue book\n4. Return book\n5. My issued books\n0. Logout\nChoice: ",subject);
        scanf("%d",&c);
        switch(c) {
            case 1: viewBooks(subject); break;
            case 2: searchBook(subject); break;
            case 3: issueBook(user,subject); break;
            case 4: returnBook(user,subject); break;
            case 5: myBooks(user); break;
            case 0: printf("Logged out.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while(c!=0);
}

int main() {
    int c;
    loadData();
    do {
        printf("\n=== LIBRARY MANAGEMENT SYSTEM ===\n1. Student Login\n2. Admin Login\n0. Exit\nChoice: ");
        scanf("%d",&c);
        switch(c) {
            case 1: student(); break;
            case 2: admin(); break;
            case 0: printf("Thank you!\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while(c!=0);
    return 0;
}
