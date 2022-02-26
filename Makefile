.PHONY: all
all:
	gcc -o rpn-calc -O3 -Wall -Wextra -Wpedantic rpn-calc.c -lm

.PHONY: debug
debug:
	gcc -o rpn-calc -g -Wall -Wextra -Wpedantic rpn-calc.c -lm
