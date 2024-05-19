#ifndef _KALLOC_BUDDY_H
#define _KALLOC_BUDDY_H
#include <kalloc/memory.h>
#include <types.h>

#define KALLOC_EMPTY  0x00
#define KALLOC_FILLED 0x01

#define KALLOC_MAX_ORDER 0x10

// typedef buddy_block_t u8; // 

u64 kalloc_init(struct memory_descriptor mmap[], u64 mmap_size, u64 desc_sz);

#endif

