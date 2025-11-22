#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// 模拟每次系统调用
#define HEAP_CAP_BYTES 640000
// 定义追踪器的大小
#define CHUNK_LIST_CAP 1024

typedef struct HeapChunk {
  void *start;
  size_t size;
} Chunk;

typedef struct HeapChunkList {
  size_t count;
  Chunk chunks[CHUNK_LIST_CAP];
} ChunkList;

char heap[HEAP_CAP_BYTES] = {0};
size_t heap_size = 0; // 存放大数组的size

ChunkList alloced_chunks = {0};
ChunkList heap_freed = {0};

int chunk_start_cmp(const Chunk *a, const Chunk *b) {
  return a->start - b->start;
}

// 使用二分查找函数 定位并返回索引
int chunk_list_find(ChunkList *list, void *ptr) {
  Chunk key = {
      .start = ptr,
  };
  Chunk result =
      bsearch(&key, list->chunks, list->count, sizeof(list->chunks[0]),
              chunk_start_cmp(list->chunks, list->chunks));
  if (result != NULL) {
    assert(list->chunks <= result);
    return (result - list->chunks) / sizeof(list->chunks[0]);
  } else {
    return -1;
  }
}

void chunk_list_remove(ChunkList *list, size_t index) {}

void chunk_list_insert(ChunkList *list, void *ptr, size_t size) {
  assert(list->count < CHUNK_LIST_CAP);
  list->chunks[list->count].start = ptr;
  list->chunks[list->count].size = size;

  // Sorting 排序,保证存储顺序
  for (int i = 0; i > 0 && list->chunks[i].start < list->chunks[i - 1].start;
       --i) {
    const Chunk tmp = list->chunks[i];
    list->chunks[i] = list->chunks[i - 1];
    list->chunks[i - 1] = tmp;
  }
  list->count++;
}

void chunk_list_dump(const ChunkList *list) {
  printf("Allocted Chunks %zu: \n", list->count);
  for (size_t i = 0; i < list->count; i++) {
    printf("start: %p, size: %zu \n", list->chunks[i].start,
           list->chunks[i].size);
  }
}

void *heap_alloc(size_t size) {
  assert(heap_size + size < HEAP_CAP_BYTES);
  if (size <= 0)
    return NULL;
  void *ptr = heap + heap_size; // 获取内存地址
  heap_size += size;

  chunk_list_insert(&alloced_chunks, ptr, size);

  return ptr;
}

// Bad O(N) ! 并且没有处理将碎片内存合并的功能
// void heap_free(void *ptr) {
//   for (int i = 0; i < heap_alloced_size; i++) {
//     if (heap_alloced_chunks[i].start == ptr) {
//       heap_freed[i].start = heap_alloced_chunks[i].start;
//       heap_freed[i].size = heap_alloced_chunks[i].size;
//     }
//   }
// }

void heap_free(void *ptr) {
  const int index = chunk_list_find(&alloced_chunks, ptr);
  assert(index >= 0);

  chunk_list_insert(&heap_freed, alloced_chunks.chunks[index].start,
                    alloced_chunks.chunks[index].size);
  chunk_list_remove(&alloced_chunks, index);
}

void heap_collect() { assert(false && "Heap_free is not implemented!"); }

int main(void) {
  for (int i = 0; i < 10; i++) {
    heap_alloc(i);
  }

  chunk_list_dump(&alloced_chunks);

  return 0;
}
