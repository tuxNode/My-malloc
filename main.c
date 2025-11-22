#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

// 模拟每次系统调用
#define HEAP_CAP_BYTES 640000
// 模拟每次调用calloc分配的内存大小
#define HEAP_ALLOCTED_CAP 1024

typedef struct HeapChunk {
  void *start;
  size_t size;
} HeapChunk;

char heap[HEAP_CAP_BYTES] = {0};
size_t heap_size = 0; // 存放大数组的size
HeapChunk heap_alloced_chunks[HEAP_ALLOCTED_CAP] = {0};
size_t heap_alloced_size = 0; // 存放已分配的size

void *heap_alloc(size_t size) {
  assert(heap_size + size < HEAP_CAP_BYTES);
  void *result = heap + heap_size;
  heap_size += size;
  const HeapChunk chunk = {
      .start = result,
      .size = size,
  };

  assert(heap_alloced_size < HEAP_ALLOCTED_CAP);
  heap_alloced_chunks[heap_alloced_size++] = chunk;

  return result;
}

void heap_dump_alloced_chunks(void) {
  printf("Allocted Chunks %zu: \n", heap_alloced_size);
  for (size_t i = 0; i < heap_alloced_size; i++) {
    printf("start: %p, size: %zu \n", heap_alloced_chunks[i].start,
           heap_alloced_chunks[i].size);
  }
}

void heap_free(void *ptr) { assert(false && "Heap_free is not implemented!"); }

void heap_collect() { assert(false && "Heap_free is not implemented!"); }

int main(void) {
  char *root = heap_alloc(26);
  for (int i = 0; i < 26; i++) {
    root[i] = 'A' + i;
  }

  heap_dump_alloced_chunks();

  // heap_free(root);

  return 0;
}
