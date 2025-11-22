# An easy malloc funtion with GC implementation

## Introduction

This proj is an easy practice to understanding allocting space from memory.

It don't really calloct memory in heap, and I did't use function like mmap() or others like that.

I use a **large ARRAY** enough to simulate a heap. BTW, just make us focus on GC.

## Resource

<https://www.youtube.com/watch?v=sZ8GJ1TiMdk&t=254s>

<https://github.com/tsoding/memalloc>

Thanks for TSODING.

## Implementation

Here are two Data Struct to track memory allocting and recording the free memories.

And I use sliding window as a filter to sift those NULL pointer or memory which is not used. 
