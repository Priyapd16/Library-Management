#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "issue.h"
#include "util.h"

static int next_issue_id(Issue *head) {
    int max_id = 0;
    for (Issue *cur = head; cur; cur = cur->next) {
        if (cur->issue_id > max_id) max_id = cur->issue_id;
    }
    return max_id + 1;
}

static void append_issue(Issue **head, Issue *node) {
    if (*head == NULL) {
        *head = node;
        return;
    }
    Issue *cur = *head;
    while (cur->next != NULL) cur = cur->next;
    cur->next = node;
}

static void format_date(time_t t, char *buf, size_t size) {
    if (t == 0) {
        snprintf(buf, size, "Not Returned");
        return;
    }
    struct tm *tm_info = localtime(&t);
    strftime(buf, size, "%d-%m-%Y", tm_info);
}

void issue_book(Book *book_head, Issue **issue_head) {
    int book_id = read_int("Enter Book ID to issue: ");
    Book *b = find_book_by_id(book_head, book_id);
    if (!b) {
        printf("No book found with ID %d.\n", book_id);
        return;
    }
    if (b->quantity <= 0) {
        printf("\"%s\" is currently out of stock.\n", b->title);
        return;
    }

    int user_id = read_int("Enter User ID: ");
    char user_name[USER_NAME_LEN];
    printf("Enter User Name: ");
    read_line(user_name, sizeof(user_name));

    Issue *node = malloc(sizeof(Issue));
    if (!node) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    node->issue_id = next_issue_id(*issue_head);
    node->book_id = b->book_id;
    strncpy(node->book_title, b->title, TITLE_LEN - 1);
    node->book_title[TITLE_LEN - 1] = '\0';
    node->user_id = user_id;
    strncpy(node->user_name, user_name, USER_NAME_LEN - 1);
    node->user_name[USER_NAME_LEN - 1] = '\0';
    node->issue_date = time(NULL);
    node->due_date = node->issue_date + (time_t)(LOAN_DAYS * 24 * 60 * 60);
    node->return_date = 0;
    node->fine_amount = 0.0f;
    node->next = NULL;

    append_issue(issue_head, node);
    b->quantity -= 1;

    char issue_str[16], due_str[16];
    format_date(node->issue_date, issue_str, sizeof(issue_str));
    format_date(node->due_date, due_str, sizeof(due_str));
    printf("Book issued. Issue ID: %d | Issue Date: %s | Due Date: %s\n",
           node->issue_id, issue_str, due_str);
}

void return_book(Issue *issue_head, Book *book_head) {
    int issue_id = read_int("Enter Issue ID to return: ");
    Issue *rec = NULL;
    for (Issue *cur = issue_head; cur; cur = cur->next) {
        if (cur->issue_id == issue_id && cur->return_date == 0) {
            rec = cur;
            break;
        }
    }
    if (!rec) {
        printf("No active (unreturned) issue record found with ID %d.\n", issue_id);
        return;
    }

    rec->return_date = time(NULL);

    double seconds_late = difftime(rec->return_date, rec->due_date);
    long days_late = 0;
    if (seconds_late > 0) {
        days_late = (long)(seconds_late / (60 * 60 * 24));
        if (seconds_late - (double)days_late * 60 * 60 * 24 > 0) days_late += 1; /* round up partial day */
    }
    rec->fine_amount = (float)(days_late * FINE_PER_DAY);

    Book *b = find_book_by_id(book_head, rec->book_id);
    if (b) b->quantity += 1;

    if (days_late > 0) {
        printf("Book returned. %ld day(s) late. Fine: Rs.%.2f\n", days_late, rec->fine_amount);
    } else {
        printf("Book returned on time. No fine.\n");
    }
}

void list_issued_books(Issue *issue_head) {
    if (!issue_head) {
        printf("No issue records yet.\n");
        return;
    }
    printf("%-4s %-6s %-22s %-6s %-15s %-11s %-11s %-13s %-6s\n",
           "IID", "BID", "Title", "UID", "User", "Issued", "Due", "Returned", "Fine");
    printf("--------------------------------------------------------------------------------------------------\n");
    for (Issue *cur = issue_head; cur; cur = cur->next) {
        char issue_str[16], due_str[16], ret_str[16];
        format_date(cur->issue_date, issue_str, sizeof(issue_str));
        format_date(cur->due_date, due_str, sizeof(due_str));
        format_date(cur->return_date, ret_str, sizeof(ret_str));
        printf("%-4d %-6d %-22s %-6d %-15s %-11s %-11s %-13s %-6.2f\n",
               cur->issue_id, cur->book_id, cur->book_title, cur->user_id,
               cur->user_name, issue_str, due_str, ret_str, cur->fine_amount);
    }
}

void save_issues(Issue *issue_head, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Could not open %s for writing.\n", filename);
        return;
    }
    for (Issue *cur = issue_head; cur; cur = cur->next) {
        fprintf(fp, "%d|%d|%s|%d|%s|%ld|%ld|%ld|%.2f\n",
                cur->issue_id, cur->book_id, cur->book_title, cur->user_id,
                cur->user_name, (long)cur->issue_date, (long)cur->due_date,
                (long)cur->return_date, cur->fine_amount);
    }
    fclose(fp);
}

void load_issues(Issue **issue_head, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    char line[400];
    while (fgets(line, sizeof(line), fp)) {
        Issue *node = malloc(sizeof(Issue));
        if (!node) { fclose(fp); return; }

        char book_title[TITLE_LEN], user_name[USER_NAME_LEN];
        int issue_id, book_id, user_id;
        long issue_date, due_date, return_date;
        float fine;

        /* format: issue_id|book_id|book_title|user_id|user_name|issue_date|due_date|return_date|fine */
        char *tok, *rest = line;
        char *fields[9];
        int i;
        for (i = 0; i < 9; i++) {
            tok = strsep(&rest, "|");
            if (!tok) break;
            fields[i] = tok;
        }
        if (i < 9) { free(node); continue; }

        issue_id = atoi(fields[0]);
        book_id = atoi(fields[1]);
        strncpy(book_title, fields[2], TITLE_LEN - 1);
        book_title[TITLE_LEN - 1] = '\0';
        user_id = atoi(fields[3]);
        strncpy(user_name, fields[4], USER_NAME_LEN - 1);
        user_name[USER_NAME_LEN - 1] = '\0';
        issue_date = atol(fields[5]);
        due_date = atol(fields[6]);
        return_date = atol(fields[7]);
        fine = (float)atof(fields[8]);

        node->issue_id = issue_id;
        node->book_id = book_id;
        strcpy(node->book_title, book_title);
        node->user_id = user_id;
        strcpy(node->user_name, user_name);
        node->issue_date = (time_t)issue_date;
        node->due_date = (time_t)due_date;
        node->return_date = (time_t)return_date;
        node->fine_amount = fine;
        node->next = NULL;

        append_issue(issue_head, node);
    }
    fclose(fp);
}

void free_issues(Issue *issue_head) {
    while (issue_head) {
        Issue *next = issue_head->next;
        free(issue_head);
        issue_head = next;
    }
}
