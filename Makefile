.PHONY: all
all:
	gcc -o rpn-calc -O3 rpn-calc.c

.PHONY: debug
debug:
	gcc -o rpn-calc -g rpn-calc.c
