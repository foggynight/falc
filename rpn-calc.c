#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE  (1 << 8)
#define STACK_SIZE (1 << 8)
#define WORD_SIZE  25

#define min(X, Y) ((X > Y) ? Y : X)
#define max(X, Y) ((X < Y) ? Y : X)

double stack[STACK_SIZE];
size_t stack_ptr = 0;

void stack_push(double n) { stack[stack_ptr++] = n; }
double stack_pop(void) { return stack[--stack_ptr]; }
void stack_clear(void) { stack_ptr = 0; }

int inter_word(const char *line, size_t word_start, size_t word_end) {
    static char str[WORD_SIZE];
    strncpy(str, line + word_start, min(word_end, 21));
    if (isdigit(str[0])
        || ((str[0] == '-' || str[0] == '+')
            && (word_end > word_start + 1)
            && isdigit(str[1]))) {
        char *end;
        double n = strtod(str, &end);
        stack_push(n);
    }
    else if (word_start == word_end - 1) {
        double e;
        switch (str[0]) {
        case '+': e = stack_pop(); stack[stack_ptr-1] += e; break;
        case '-': e = stack_pop(); stack[stack_ptr-1] -= e; break;
        case '*': e = stack_pop(); stack[stack_ptr-1] *= e; break;
        case '/': e = stack_pop(); stack[stack_ptr-1] /= e; break;
        }
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
    putchar('(');
    if (stack_ptr > 0)
        putchar(' ');
    for (size_t i = 0; i < stack_ptr; ++i)
        printf("%g ", stack[i]);
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
