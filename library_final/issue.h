#ifndef ISSUE_H
#define ISSUE_H

#include <time.h>
#include "book.h"

#define USER_NAME_LEN 100
#define FINE_PER_DAY  5.0   /* rupees per day late */
#define LOAN_DAYS     7     /* default loan period */

/* Issue record - stored as a singly linked list node.
 * return_date == 0 means the book has not been returned yet. */
typedef struct Issue {
	int issue_id;
	int book_id;
	char book_title[TITLE_LEN];
	int user_id;
	char user_name[USER_NAME_LEN];
	time_t issue_date;
	time_t due_date;
	time_t return_date;
	float fine_amount;
	struct Issue *next;
} Issue;

void issue_book(Book *book_head, Issue **issue_head);
void return_book(Issue *issue_head, Book *book_head);
void list_issued_books(Issue *issue_head);

void save_issues(Issue *issue_head, const char *filename);
void load_issues(Issue **issue_head, const char *filename);

void free_issues(Issue *issue_head);

#endif
