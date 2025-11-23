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
// size_t heap_size = 0; // 存放大数组的size

typedef struct HeapChunk {
  void *start;
  size_t size;
} Chunk;

typedef struct HeapChunkList {
  size_t count;
  Chunk chunks[CHUNK_LIST_CAP];
} ChunkList;

ChunkList alloced_chunks = {0};
ChunkList heap_freed = {
    .count = 1,
    .chunks = {[0] = {.start = heap, .size = sizeof(heap)}},
};

int chunk_list_insert(ChunkList *list, void *ptr, size_t size) {
  if (list->count >= CHUNK_LIST_CAP)
    return -1;

  list->chunks[list->count].start = ptr;
  list->chunks[list->count].size = size;

  // 实现排序，为实现合并空内存做准备
  for (size_t i = list->count;
       i > 0 && list->chunks[i].start < list->chunks[i - 1].start; --i) {
    const Chunk tmp = list->chunks[i];

    list->chunks[i] = list->chunks[i - 1];
    list->chunks[i - 1] = tmp;
  }
  list->count++;

  return list->count;
}

int chunk_cmp(const void *a, const void *b) {
  const Chunk *a_chunk = a;
  const Chunk *b_chunk = b;
  if (a_chunk->start < b_chunk->start)
    return -1;
  if (a_chunk->start > b_chunk->start)
    return 1;
  return 0;
}

int chunk_list_find(ChunkList *list, void *ptr) {
  // 实现二分查找
  Chunk key = {
      .start = ptr,
  };
  Chunk *result = bsearch(&key, list->chunks, list->count,
                          sizeof(alloced_chunks.chunks[0]), chunk_cmp);

  if (result != NULL) {
    assert(list->chunks <= result);
    return result - list->chunks;
  } else
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

  for (size_t i = 0; i < heap_freed.count; i++) {
    if (heap_freed.chunks[i].size >= size) {
      const Chunk chunk = heap_freed.chunks[i];
      if (chunk.size >= size) {
        chunk_list_remove(&heap_freed, i);
        chunk_list_insert(&alloced_chunks, chunk.start, size);

        // 切分内存块，将未分配的块中剩余内存记录大小
        const size_t tail_size = chunk.size - size;

        if (tail_size > 0) {
          chunk_list_insert(&heap_freed, (char *)chunk.start + size, tail_size);
        }

        return chunk.start;
      }
    }
  }
  return NULL;
}

void heap_free(void *ptr) {
  if (ptr == NULL)
    return;

  int index = chunk_list_find(&alloced_chunks, ptr);
  if (index == -1) {
    fprintf(stderr, "Error: Attempt to free non-allocated pointer %p\n", ptr);
    return;
  }

  // TODO:: implement a memory comflact
  size_t size = alloced_chunks.chunks[index].size;

  chunk_list_insert(&heap_freed, alloced_chunks.chunks[index].start, size);
  chunk_list_remove(&alloced_chunks, (size_t)index);

  printf("Freed memory at %p, size: %zu\n", ptr, size);
}

void chunk_list_dump(ChunkList *list, ChunkList *freelist) {
  printf("ChunkList : %zu \n", list->count);
  for (size_t i = 0; i < list->count; i++) {
    printf("start; %p  size: %zu\n", list->chunks[i].start,
           list->chunks[i].size);
  }
  printf("Freed ChunkList: %zu\n", freelist->count);
  for (size_t i = 0; i < freelist->count; i++) {
    printf("start; %p  size: %zu\n", freelist->chunks[i].start,
           freelist->chunks[i].size);
  }
}

int main(void) {
  void *tab[10];
  chunk_list_dump(&alloced_chunks, &heap_freed);
  for (int i = 1; i <= 10; i++) {
    tab[i] = heap_alloc(i);
  }
  chunk_list_dump(&alloced_chunks, &heap_freed);
  heap_free(tab[5]);
  chunk_list_dump(&alloced_chunks, &heap_freed);
}
