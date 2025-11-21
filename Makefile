CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb

heap: main.c
	cc $(CFLAGS) main.c -o heap
