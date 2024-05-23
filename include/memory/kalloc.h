#ifndef _MEMORY_KALLOC_H
#define _MEMORY_KALLOC_H
#include <kalloc/memory.h>
#include <types.h>

#define KALLOC_EMPTY        0x00
#define KALLOC_RIGHT_FILLED 0x01
#define KALLOC_LEFT_FILLED  0x02
#define KALLOC_FILLED       0x03

u64 kalloc_init(struct memory_descriptor mmap[], u64 mmap_size, u64 desc_sz);

#endif

