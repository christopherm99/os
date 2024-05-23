#ifndef _MEMORY_INIT_H
#define _MEMORY_INIT_H
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

void kmem_init(void *rbp, struct memory_descriptor mmap[], u64 mmap_size, u64 desc_sz);

#endif

