// rpn-calc.c - Reverse Polish Notation calculator.
// Copyright (C) 2022 Robert Coffey
// Released under the GPLv3.

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICT_SIZE  ('z' - 'a' + 1)
#define LINE_SIZE  (1 << 8)
#define STACK_SIZE (1 << 8)
#define KEY_SIZE   1
#define WORD_SIZE  (1 << 9)

#define ERR_UF 1 // stack underflow
#define ERR_OF 2 // stack overflow
#define ERR_IK 3 // invalid key

#define min(X, Y) ((X > Y) ? Y : X)
#define max(X, Y) ((X < Y) ? Y : X)

double stack[STACK_SIZE];
size_t stack_ptr = 0;

size_t stack_size(void) { return stack_ptr; }
void stack_clear(void) { stack_ptr = 0; }
void stack_push(double n) { stack[stack_ptr++] = n; }
double stack_pop(void) { return stack[--stack_ptr]; }
double stack_top(void) { return stack[stack_ptr - 1]; }
double stack_offset(size_t offset) { return stack[stack_ptr - offset - 1]; }

#define handle_uf(X) if (stack_size() < (X)) return ERR_UF
#define handle_of(X) if (stack_size() + (X) >= STACK_SIZE) return ERR_OF
#define handle_uof(X, Y)                                        \
    if (stack_size() < (X)) return ERR_UF;                      \
    else if (stack_size() + (Y) >= STACK_SIZE) return ERR_OF    \

double dict[DICT_SIZE];

double dict_get(char key) { return dict[tolower(key) % DICT_SIZE]; }
void dict_set(char key, double n) { dict[tolower(key) % DICT_SIZE] = n; }

enum state { NONE, STORE } state = NONE;

int inter_word(const char *line, size_t word_start, size_t word_end) {
    static char word[WORD_SIZE];
    const size_t word_len = word_end - word_start;
    const size_t word_lim = min(word_len, WORD_SIZE - 1);
    strncpy(word, line + word_start, word_lim);
    word[word_lim] = '\0';

    switch (state) {
    case NONE: // run number or execute command
        // word contains a number
        if (isdigit(word[0])
            || ((word[0] == '-' || word[0] == '+')
                && (word_end > word_start + 1)
                && isdigit(word[1]))) {
            if (stack_size() >= STACK_SIZE)
                return ERR_OF;
            char *end;
            double n = strtod(word, &end);
            stack_push(n);
        }
        // word contains a single character
        else if (word_start == word_end - 1) {
            if (isalpha(word[0])) {
                if (stack_size() >= STACK_SIZE)
                    return ERR_OF;
                stack_push(dict_get(word[0]));
            }
            else {
                if (stack_size() < 2)
                    return ERR_UF;
                double e;
                switch (word[0]) {
                case '+': e = stack_pop(); stack[stack_ptr-1] += e; break;
                case '-': e = stack_pop(); stack[stack_ptr-1] -= e; break;
                case '*': e = stack_pop(); stack[stack_ptr-1] *= e; break;
                case '/': e = stack_pop(); stack[stack_ptr-1] /= e; break;
                }
            }
        }
        else { // word contains multiple characters
            if (!strcmp(word, "store"))
            { handle_uf(1); state = STORE; }
            else if (!strcmp(word, "dup"))
            { handle_uof(1, 1); stack_push(stack_top()); }
            else if (!strcmp(word, "over"))
            { handle_uof(2, 1); stack_push(stack_offset(1)); }
            else if (!strcmp(word, "drop"))
            { handle_uf(1); stack_pop(); }
            else if (!strcmp(word, "clear"))
            { stack_clear(); }
            else if (!strcmp(word, "sqrt"))
            { handle_uf(1); stack_push(sqrt(stack_pop())); }
            else if (!strcmp(word, "ln"))
            { handle_uf(1); stack_push(log(stack_pop())); }
            else if (!strcmp(word, "floor"))
            { handle_uf(1); stack_push(floor(stack_pop())); }
            else if (!strcmp(word, "ceil"))
            { handle_uf(1); stack_push(ceil(stack_pop())); }
            else if (!strcmp(word, "round"))
            { handle_uf(1); stack_push(round(stack_pop())); }
            else if (!strcmp(word, "pi"))
            { handle_of(1); stack_push(M_PI); }
            else if (!strcmp(word, "tau"))
            { handle_of(1); stack_push(2 * M_PI); }
            else if (!strcmp(word, "sin"))
            { handle_uf(1); stack_push(sin(stack_pop())); }
            else if (!strcmp(word, "cos"))
            { handle_uf(1); stack_push(cos(stack_pop())); }
            else if (!strcmp(word, "tan"))
            { handle_uf(1); stack_push(tan(stack_pop())); }
            else if (!strcmp(word, "pow"))
            {
                handle_uf(2);
                double e = stack_pop();
                stack_push(pow(stack_pop(), e));
            }
            else if (!strcmp(word, "root"))
            {
                handle_uf(2);
                double e = stack_pop();
                stack_push(pow(stack_pop(), 1.0 / e));
            }
            else if (!strcmp(word, "log"))
            {
                handle_uf(2);
                double e = stack_pop();
                stack_push(log(stack_pop()) / log(e));
            }
        }
        break;
    case STORE: // store value in dictionary
        state = NONE;
        if (word_len > KEY_SIZE || !isalpha(word[0]))
            return ERR_IK;
        else dict_set(word[0], stack_pop());
        break;
    }

    return 0;
}

int inter_line(const char *line, size_t line_len) {
    size_t word_start = 0, word_end = 0, err = 0;
    for (size_t i = 0; i < line_len; ++i) {
        if (!isspace(line[i])) {
            word_start = i;
            for (word_end = word_start + 1;
                 word_end < line_len
                     && !isspace(line[word_end]);
                 ++word_end);
            if ((err = inter_word(line, word_start, word_end)))
                return err;
            i = word_end;
        }
    }
    return 0;
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
        switch (inter_line(line, strlen(line))) {
        case 1: puts("stack underflow"); break;
        case 2: puts("stack overflow"); break;
        case 3: puts("invalid key"); break;
        }
        print_stack();
    }
    return 0;
}
