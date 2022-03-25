.PHONY: all
all:
	gcc -o falc -O3 -Wall -Wextra -Wpedantic falc.c -lm

.PHONY: debug
debug:
	gcc -o falc -g -Wall -Wextra -Wpedantic falc.c -lm

.PHONY: install
install:
	cp falc /usr/local/bin

.PHONY: uninstall
uninstall:
	rm /usr/local/bin/falc
