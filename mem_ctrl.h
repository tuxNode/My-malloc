#ifndef MEM_CTRL
#define MEM_CTRL

#include <stdio.h>
// 定义内存块大小
#define HEAP_CAP_BYTES 640000
// 定义追踪器的大小
#define CHUNK_LIST_CAP 1024
#define HEAP_ALLOCTED_CAP 1024

typedef struct HeapChunk {
  void *start;
  size_t size;
} Chunk;

typedef struct HeapChunkList {
  size_t count;
  Chunk chunks[CHUNK_LIST_CAP];
} ChunkList;

// Prototype
int chunk_list_insert(ChunkList *list, void *ptr, size_t size);
int chunk_list_find(ChunkList *list, void *ptr);
void chunk_list_remove(ChunkList *list, size_t index);
void chunk_list_merge(ChunkList *dst, ChunkList *src);
void *heap_alloc(size_t size);
void heap_free(void *ptr);
void chunk_list_dump(ChunkList *list, ChunkList *freelist);

#endif
