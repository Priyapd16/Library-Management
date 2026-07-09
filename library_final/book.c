#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h"
#include "util.h"
Book *find_book_by_id(Book *head, int id) {
    for (Book *cur = head; cur != NULL; cur = cur->next) {
        if (cur->book_id == id) return cur;
    }
    return NULL;
}
Book *find_book_by_title(Book *head, const char *title) {
    for (Book *cur = head; cur != NULL; cur = cur->next) {
        if (contains_ci(cur->title, title)) return cur;
    }
    return NULL;
}
static Book *make_book_node(int id, const char *title, const char *author, int qty) {
    Book *b = malloc(sizeof(Book));
    if (!b) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    b->book_id = id;
    strncpy(b->title, title, TITLE_LEN - 1);
    b->title[TITLE_LEN - 1] = '\0';
    strncpy(b->author, author, AUTHOR_LEN - 1);
    b->author[AUTHOR_LEN - 1] = '\0';
    b->quantity = qty;
    b->next = NULL;
    return b;
}
/* Append so the books are arranged in order*/
static void append_book(Book **head, Book *node) {
    if (*head == NULL) {
        *head = node;
        return;
    }
    Book *cur = *head;
    while (cur->next != NULL) cur = cur->next;
    cur->next = node;
}

void add_book(Book **head) {
    int id = read_int("Enter Book ID: ");
    if (find_book_by_id(*head, id) != NULL) {
        printf("A book with ID %d already exists.\n", id);
        return;
    }

    char title[TITLE_LEN], author[AUTHOR_LEN];
    printf("Enter Title: ");
    read_line(title, sizeof(title));
    printf("Enter Author: ");
    read_line(author, sizeof(author));
    int qty = read_int("Enter Quantity: ");
    if (qty < 0) {
        printf("Quantity cannot be negative. Set to 0.\n");
        qty = 0;
    }

    Book *node = make_book_node(id, title, author, qty);
    append_book(head, node);
    printf("Book added successfully.\n");
}

static void edit_book_fields(Book *b) {
    char buf[TITLE_LEN];
    printf("Current Title : %s\n", b->title);
    printf("New Title (blank to keep): ");
    read_line(buf, sizeof(buf));
    if (buf[0] != '\0') {
        strncpy(b->title, buf, TITLE_LEN - 1);
        b->title[TITLE_LEN - 1] = '\0';
    }

    printf("Current Author: %s\n", b->author);
    printf("New Author (blank to keep): ");
    read_line(buf, sizeof(buf));
    if (buf[0] != '\0') {
        strncpy(b->author, buf, AUTHOR_LEN - 1);
        b->author[AUTHOR_LEN - 1] = '\0';
    }

    printf("Current Quantity: %d\n", b->quantity);
    int qty = read_int("New Quantity (-1 to keep): ");
    if (qty >= 0) {
        b->quantity = qty;
    }

    printf("Book updated successfully.\n");
}

void update_book_menu(Book **head) {
    printf("\nUpdate Book Details\nA. By Book ID\nB. By Book Name\nC. Back to Main Menu\nChoice: ");
    char choice[16];
    read_line(choice, sizeof(choice));

    if (choice[0] == 'A' || choice[0] == 'a') {
        int id = read_int("Enter Book ID: ");
        Book *b = find_book_by_id(*head, id);
        if (!b) { printf("Book not found.\n"); return; }
        edit_book_fields(b);
    } else if (choice[0] == 'B' || choice[0] == 'b') {
        char name[TITLE_LEN];
        printf("Enter Book Name: ");
        read_line(name, sizeof(name));
        Book *b = find_book_by_title(*head, name);
        if (!b) { printf("Book not found.\n"); return; }
        edit_book_fields(b);
    } else {
        return; /* back to main menu */
    }
}

static void unlink_book(Book **head, Book *target) {
    if (*head == target) {
        *head = target->next;
        free(target);
        return;
    }
    Book *cur = *head;
    while (cur->next != NULL && cur->next != target) cur = cur->next;
    if (cur->next == target) {
        cur->next = target->next;
        free(target);
    }
}

void remove_book_menu(Book **head) {
    printf("\nRemove Book\nA. By Book ID\nB. By Book Name\nC. Back to Main Menu\nChoice: ");
    char choice[16];
    read_line(choice, sizeof(choice));

    Book *target = NULL;
    if (choice[0] == 'A' || choice[0] == 'a') {
        int id = read_int("Enter Book ID: ");
        target = find_book_by_id(*head, id);
    } else if (choice[0] == 'B' || choice[0] == 'b') {
        char name[TITLE_LEN];
        printf("Enter Book Name: ");
        read_line(name, sizeof(name));
        target = find_book_by_title(*head, name);
    } else {
        return;
    }

    if (!target) {
        printf("Book not found.\n");
        return;
    }
    printf("Removing \"%s\" by %s. Are you sure? (y/n): ", target->title, target->author);
    char confirm[8];
    read_line(confirm, sizeof(confirm));
    if (confirm[0] == 'y' || confirm[0] == 'Y') {
        unlink_book(head, target);
        printf("Book removed.\n");
    } else {
        printf("Cancelled.\n");
    }
}

static void print_book_row(const Book *b) {
    printf("%-6d %-30s %-20s %-8d\n", b->book_id, b->title, b->author, b->quantity);
}

static void print_header(void) {
    printf("%-6s %-30s %-20s %-8s\n", "ID", "Title", "Author", "Qty");
    printf("---------------------------------------------------------------------\n");
}

void search_book_menu(Book *head) {
    printf("\nSearch Book\nA. By Book ID\nB. By Book Name\nC. By Author Name\nD. Back to Main Menu\nChoice: ");
    char choice[16];
    read_line(choice, sizeof(choice));

    if (choice[0] == 'A' || choice[0] == 'a') {
        int id = read_int("Enter Book ID: ");
        Book *b = find_book_by_id(head, id);
        if (!b) { printf("No book found with ID %d.\n", id); return; }
        print_header();
        print_book_row(b);
    } else if (choice[0] == 'B' || choice[0] == 'b') {
        char name[TITLE_LEN];
        printf("Enter Book Name (or part of it): ");
        read_line(name, sizeof(name));
        int found = 0;
        print_header();
        for (Book *cur = head; cur; cur = cur->next) {
            if (contains_ci(cur->title, name)) { print_book_row(cur); found = 1; }
        }
        if (!found) printf("No matching books.\n");
    } else if (choice[0] == 'C' || choice[0] == 'c') {
        char author[AUTHOR_LEN];
        printf("Enter Author Name (or part of it): ");
        read_line(author, sizeof(author));
        int found = 0;
        print_header();
        for (Book *cur = head; cur; cur = cur->next) {
            if (contains_ci(cur->author, author)) { print_book_row(cur); found = 1; }
        }
        if (!found) printf("No matching books.\n");
    } else {
        return;
    }
}

void view_all_books(Book *head) {
    if (!head) {
        printf("No books in the library yet.\n");
        return;
    }
    print_header();
    for (Book *cur = head; cur; cur = cur->next) {
        print_book_row(cur);
    }
}

void save_books(Book *head, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Could not open %s for writing.\n", filename);
        return;
    }
    for (Book *cur = head; cur; cur = cur->next) {
        fprintf(fp, "%d|%s|%s|%d\n", cur->book_id, cur->title, cur->author, cur->quantity);
    }
    fclose(fp);
}

void load_books(Book **head, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return; /* no file yet on first run - not an error */

    char line[300];
    while (fgets(line, sizeof(line), fp)) {
        int id, qty;
        char title[TITLE_LEN], author[AUTHOR_LEN];
        /* format: id|title|author|qty */
        char *p = line;
        char *pipe1 = strchr(p, '|');
        if (!pipe1) continue;
        *pipe1 = '\0';
        id = atoi(p);

        char *pipe2 = strchr(pipe1 + 1, '|');
        if (!pipe2) continue;
        *pipe2 = '\0';
        strncpy(title, pipe1 + 1, TITLE_LEN - 1);
        title[TITLE_LEN - 1] = '\0';

        char *pipe3 = strchr(pipe2 + 1, '|');
        if (!pipe3) continue;
        *pipe3 = '\0';
        strncpy(author, pipe2 + 1, AUTHOR_LEN - 1);
        author[AUTHOR_LEN - 1] = '\0';

        qty = atoi(pipe3 + 1);

        Book *node = make_book_node(id, title, author, qty);
        append_book(head, node);
    }
    fclose(fp);
}
void free_books(Book *head) {
    while (head) {
        Book *next = head->next;
        free(head);
        head = next;
    }
}
