#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE  (1 << 8)
#define STACK_SIZE (1 << 8)

#define min(X, Y) ((X > Y) ? Y : X)
#define max(X, Y) ((X < Y) ? Y : X)

long stack[STACK_SIZE];
size_t stack_ptr = 0;

void stack_push(long n) { stack[stack_ptr++] = n; }
long stack_pop(void) { return stack[--stack_ptr]; }
void stack_clear(void) { stack_ptr = 0; }

int inter_word(const char *line, size_t word_start, size_t word_end) {
    static char str[21];
    strncpy(str, line + word_start, min(word_end, 21));
    if (isdigit(line[word_start])) {
        char *end;
        long n = strtol(str, &end, 10);
        stack_push(n);
    }
}

int inter_line(const char *line, size_t line_len) {
    size_t word_start = 0, word_end = 0;
    for (size_t i = 0; i < line_len; ++i) {
        if (!isspace(line[i])) {
            word_start = i;
            for (word_end = word_start + 1;
                 word_end < line_len
                     && !isspace(line[word_end]);
                 ++word_end);
            inter_word(line, word_start, word_end);
            i = word_end;
        }
    }
    if (word_start == word_end) stack_clear();
}

void print_stack(void) {
    printf("( ");
    for (size_t i = 0; i < stack_ptr; ++i)
        printf("%d ", stack[i]);
    printf(") ");
}

int main(void) {
    char line[LINE_SIZE];
    print_stack();
    while (fgets(line, LINE_SIZE, stdin)) {
        inter_line(line, strlen(line));
        print_stack();
    }
    return 0;
}
