#include <stdio.h>
#include "book.h"
#include "issue.h"
#include "util.h"

#define BOOKS_FILE  "data/books.txt"
#define ISSUES_FILE "data/issued.txt"

static void print_menu(void) {
    printf("\n+------------------------------------------+\n");
    printf("|         Book Management Menu             |\n");
    printf("|------------------------------------------|\n");
    printf("| 1. Add New Book                          |\n");
    printf("| 2. Update Book Details                   |\n");
    printf("| 3. Remove Book                           |\n");
    printf("| 4. Search Book                           |\n");
    printf("| 5. View All Books                        |\n");
    printf("| 6. Issue Book                            |\n");
    printf("| 7. Return Book                           |\n");
    printf("| 8. List Issued Books                     |\n");
    printf("| 9. Save                                  |\n");
    printf("| 10. Exit                                 |\n");
    printf("+------------------------------------------+\n");
}

int main(void) {
    Book *book_head = NULL;
    Issue *issue_head = NULL;

    load_books(&book_head, BOOKS_FILE);
    load_issues(&issue_head, ISSUES_FILE);

    int running = 1;
    while (running) {
        print_menu();
        int choice = read_int("Enter choice: ");

        switch (choice) {
            case 1: add_book(&book_head); break;
            case 2: update_book_menu(&book_head); break;
            case 3: remove_book_menu(&book_head); break;
            case 4: search_book_menu(book_head); break;
            case 5: view_all_books(book_head); break;
            case 6: issue_book(book_head, &issue_head); break;
            case 7: return_book(issue_head, book_head); break;
            case 8: list_issued_books(issue_head); break;
            case 9:
                save_books(book_head, BOOKS_FILE);
                save_issues(issue_head, ISSUES_FILE);
                printf("Data saved.\n");
                break;
            case 10:
                save_books(book_head, BOOKS_FILE);
                save_issues(issue_head, ISSUES_FILE);
                printf("Data saved. Goodbye!\n");
                running = 0;
                break;
            default:
                printf("Invalid choice. Please select 1-10.\n");
        }
    }

    free_books(book_head);
    free_issues(issue_head);
    return 0;
}
