#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define HEAP_CAP_BYTES 640000

char heap[HEAP_CAP_BYTES] = {0};
size_t heap_size = 0;

void *heap_alloc(size_t size) {
  assert(heap_size + size < HEAP_CAP_BYTES);
  void *result = heap + heap_size;
  heap_size += size;
  return NULL;
}

void heap_free(void *ptr) { assert(false && "Heap_free is not implemented!"); }

void heap_collect() { assert(false && "Heap_free is not implemented!"); }

int main() {
  char *root = heap_alloc(26);
  for (int i = 0; i < 26; i++) {
    root[i] = 'A' + i;
  }

  return 0;
}
