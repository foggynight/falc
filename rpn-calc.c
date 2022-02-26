#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICT_SIZE  ('z' - 'a' + 1)
#define LINE_SIZE  (1 << 8)
#define STACK_SIZE (1 << 8)
#define WORD_SIZE  (1 << 9)

#define min(X, Y) ((X > Y) ? Y : X)
#define max(X, Y) ((X < Y) ? Y : X)

double stack[STACK_SIZE];
size_t stack_ptr = 0;

void stack_push(double n) { if (stack_ptr < STACK_SIZE) stack[stack_ptr++] = n;}
double stack_pop(void) { return !stack_ptr ? 0 : stack[--stack_ptr]; }
void stack_clear(void) { stack_ptr = 0; }

enum dict_state { FETCH, STORE };
enum dict_state state = FETCH;

double dict[DICT_SIZE];

double dict_get(char key) { return dict[key % DICT_SIZE]; }
void dict_set(char key, double n) { dict[key % DICT_SIZE] = n; }

int inter_word(const char *line, size_t word_start, size_t word_end) {
    static char word[WORD_SIZE];
    const size_t word_len = word_end - word_start;
    strncpy(word, line + word_start, min(word_len, WORD_SIZE-1));
    word[word_len] = '\0';
    if (isdigit(word[0])
        || ((word[0] == '-' || word[0] == '+')
            && (word_end > word_start + 1)
            && isdigit(word[1]))) {
        char *end;
        double n = strtod(word, &end);
        stack_push(n);
    }
    else if (word_start == word_end - 1) {
        if (isalpha(word[0])) {
            if (state == STORE) {
                dict_set(word[0], stack_pop());
                state = FETCH;
            }
            else {
                stack_push(dict_get(word[0]));
            }
        }
        else {
            double e;
            switch (word[0]) {
            case '+': e = stack_pop(); stack[stack_ptr-1] += e; break;
            case '-': e = stack_pop(); stack[stack_ptr-1] -= e; break;
            case '*': e = stack_pop(); stack[stack_ptr-1] *= e; break;
            case '/': e = stack_pop(); stack[stack_ptr-1] /= e; break;
            }
        }
    }
    else {
        if      (!strcmp(word, "clear")) { stack_clear(); }
        else if (!strcmp(word, "store")) { state = STORE; }
        else if (!strcmp(word, "sqrt"))  { stack_push(sqrt(stack_pop())); }
        else if (!strcmp(word, "ln"))    { stack_push(log(stack_pop())); }
        else if (!strcmp(word, "floor")) { stack_push(floor(stack_pop())); }
        else if (!strcmp(word, "ceil"))  { stack_push(ceil(stack_pop())); }
        else if (!strcmp(word, "round")) { stack_push(round(stack_pop())); }
        else if (!strcmp(word, "pi"))    { stack_push(M_PI); }
        else if (!strcmp(word, "tau"))   { stack_push(2 * M_PI); }
        else if (!strcmp(word, "sin"))   { stack_push(sin(stack_pop())); }
        else if (!strcmp(word, "cos"))   { stack_push(cos(stack_pop())); }
        else if (!strcmp(word, "tan"))   { stack_push(tan(stack_pop())); }
        else if (!strcmp(word, "pow")) {
            double e = stack_pop();
            stack_push(pow(stack_pop(), e));
        }
        else if (!strcmp(word, "root")) {
            double e = stack_pop();
            stack_push(pow(stack_pop(), 1.0 / e));
        }
        else if (!strcmp(word, "log")) {
            double e = stack_pop();
            stack_push(log(stack_pop()) / log(e));
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
