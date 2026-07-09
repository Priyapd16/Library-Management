# Library Management System (C)

A command-line Library Management System written in C, using linked lists
for storage and plain text files for persistence.

## Features

- Add, update, remove, and search books (by ID, title, or author)
- Issue and return books with automatic due-date tracking (7-day loan period)
- Late-return fine calculation (₹5/day)
- View all books and list all issue records
- Data automatically saved to and loaded from text files

## Project Structure

```
lms/
├── main.c # Menu loop
├── book.h/.c # Book struct + CRUD operations
├── issue.h/.c # Issue/return logic + fine calculation
├── util.h/.c # Shared safe-input helpers
├── Makefile # Build rules
└── data/ # books.txt and issued.txt (created at runtime)
```

## Build & Run

```bash
make
./exe
```

## Menu

```
1. Add New Book
2. Update Book Details
3. Remove Book
4. Search Book
5. View All Books
6. Issue Book
7. Return Book
8. List Issued Books
9. Save
10. Exit
```

## Data Format

Books (`data/books.txt`): `id|title|author|quantity`

Issues (`data/issued.txt`): `issue_id|book_id|title|user_id|user_name|issue_date|due_date|return_date|fine`
(dates stored as Unix timestamps, formatted as `DD-MM-YYYY` when displayed)
