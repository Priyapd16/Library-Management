#ifndef UTIL_H
#define UTIL_H

/* Reads a whole line safely into buf (size bytes), strips the
 * trailing newline. Clears any leftover input on overflow. */
void read_line(char *buf, int size);

/* Reads an integer safely, re-prompting on bad input.
 * Prints `prompt` before reading. */
int read_int(const char *prompt);

/* Case-insensitive substring search: returns 1 if needle is found
 * inside haystack, ignoring case, else 0. */
int contains_ci(const char *haystack, const char *needle);

#endif
