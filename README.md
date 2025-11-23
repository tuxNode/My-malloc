# An easy malloc funtion with GC implementation

## Introduction

This proj is an simple implementation to understanding allocting space from memory.

It don't really alloct memory in heap, and I did't use function like mmap() or others like that.

I use a **large ARRAY** enough to simulate a heap. BTW, just make us focus on GC.

Actually, the real heap is more complex than my own struct.

## Resource

<https://www.youtube.com/watch?v=sZ8GJ1TiMdk&t=254s>

<https://github.com/tsoding/memalloc>

Thanks for TSODING.

## Implementation

Here are two Data Struct to track memory allocation and recording the free memory blocks.

And I use sliding window as a filter to sift those NULL pointer or unused memory blocks. 

## Limitation

1. Could alloct a memory which the size is 0 ! (Malloc in glibc could return a only pointer, it also could be freed)
