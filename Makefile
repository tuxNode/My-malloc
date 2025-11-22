CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb -gdwarf-4

# CC=aarch64-apple-darwin24-gcc-15
CC=gcc

heap: main.c
	$(CC) $(CFLAGS) main.c -o heap
