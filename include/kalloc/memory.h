#ifndef _KALLOC_MEMORY_H
#define _KALLOC_MEMORY_H
#include <types.h>

#define NEXT_DESC(Ptr,Size)  ((struct memory_descriptor *) (((u8 *) Ptr) + Size))

struct memory_descriptor {
  u32 type;
  u32 pad;
  u64 physical_addr;
  u64 virtual_addr;
  u64 n_pages;
  u64 attr;
};

#endif

