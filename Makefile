CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb -gdwarf-4

# CC=aarch64-apple-darwin24-gcc-15
CC=gcc

FILES=mem_ctrl.c 
heap: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o heap
