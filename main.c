#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// 模拟每次系统调用
#define HEAP_CAP_BYTES 640000
// 定义追踪器的大小
#define CHUNK_LIST_CAP 1024
#define HEAP_ALLOCTED_CAP 1024

char heap[HEAP_CAP_BYTES] = {0};
size_t heap_size = 0; // 存放大数组的size

typedef struct HeapChunk {
  void *start;
  size_t size;
} Chunk;

typedef struct HeapChunkList {
  size_t count;
  Chunk chunks[CHUNK_LIST_CAP];
} ChunkList;

// 使用链表记录空闲内存
typedef struct FreeChunk {
  void *start;
  size_t size;
  struct FreeNode *next;
} FreeChunk;

FreeChunk *free_list = NULL;

ChunkList alloced_chunks = {0};
ChunkList heap_freed = {0};

int chunk_list_insert(ChunkList *list, void *ptr, size_t size) {
  if (list->count >= CHUNK_LIST_CAP)
    return -1;

  list->chunks[list->count].start = ptr;
  list->chunks[list->count].size = size;
  list->count++;

  return list->count;
}

// Bad O(N)
int chunk_list_find(ChunkList *list, void *ptr) {
  for (size_t i = 0; i < list->count; i++) {
    if (list->chunks[i].start == ptr) {
      return i;
    }
  }
  return -1;
}

void chunk_list_remove(ChunkList *list, size_t index) {
  if (index >= list->count)
    return;

  for (size_t i = index; i < list->count - 1; i++) {
    list->chunks[i] = list->chunks[i + 1];
  }
  list->count--;
}

void *heap_alloc(size_t size) {
  if (size <= 0)
    return NULL;
  if (heap_size + size > HEAP_CAP_BYTES)
    return NULL;
  if (alloced_chunks.count >= CHUNK_LIST_CAP)
    return NULL;

  void *ptr = heap + heap_size;
  chunk_list_insert(&alloced_chunks, ptr, size);
  heap_size += size;
  return ptr;
}

void heap_free(void *ptr) {
  if (ptr == NULL)
    return;

  int index = chunk_list_find(&alloced_chunks, ptr);
  if (index == -1) {
    fprintf(stderr, "Error: Attempt to free non-allocated pointer %p\n", ptr);
    return;
  }

  size_t size = alloced_chunks.chunks[index].size;

  chunk_list_insert(&heap_freed, ptr, size);
  chunk_list_remove(&alloced_chunks, index);

  printf("Freed memory at %p, size: %zu\n", ptr, size);
}

int main(void) {
  void *root = heap_alloc(20);
  heap_free(root);
}
