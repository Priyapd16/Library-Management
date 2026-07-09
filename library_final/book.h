#ifndef BOOK_H
#define BOOK_H
#define TITLE_LEN   100
#define AUTHOR_LEN  100
/* Book record - stored as a singly linked list node */
typedef struct Book {
	int book_id;
	char title[TITLE_LEN];
	char author[AUTHOR_LEN];
	int quantity;
	struct Book *next;
} Book;
/* Menu-driven operations (each handles its own I/O) */
void add_book(Book **head);
void update_book_menu(Book **head);
void remove_book_menu(Book **head);
void search_book_menu(Book *head);
void view_all_books(Book *head);
/* Lookup helpers, used by book.c and issue.c */
Book *find_book_by_id(Book *head, int id);
Book *find_book_by_title(Book *head, const char *title);
/* Persistence */
void save_books(Book *head, const char *filename);
void load_books(Book **head, const char *filename);
/* Cleanup */
void free_books(Book *head);
#endif
