#include <stdio.h>/* to combine both book.c and issue.c*/
#include <string.h>
#include <ctype.h>
#include "util.h"

void read_line(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {
        /* line was longer than buf: flush the rest of stdin */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
}

int read_int(const char *prompt) {
    char line[64];
    int value;
    while (1) {
        printf("%s", prompt);
        read_line(line, sizeof(line));
        if (sscanf(line, "%d", &value) == 1) {
            return value;
        }
        printf("Invalid number, please try again.\n");
    }
}

int contains_ci(const char *haystack, const char *needle) {
    if (*needle == '\0') return 1;
    for (const char *h = haystack; *h; h++) {
        const char *hh = h, *nn = needle;
        while (*hh && *nn && tolower((unsigned char)*hh) == tolower((unsigned char)*nn)) {
            hh++;
            nn++;
        }
        if (*nn == '\0') return 1;
    }
    return 0;
}
