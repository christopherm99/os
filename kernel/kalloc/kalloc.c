#include <kalloc/kalloc.h>
#include <kalloc/memory.h>
#include <types.h>
#include <io.h>

struct {
  u8    max_depth;
  void *heap;
} kmem;

u64 kalloc_init(struct memory_descriptor *mmap, u64 mmap_sz, u64 desc_sz) {
  printf("PHYSICAL ADDR    VIRTUAL ADDR     NPAGES   TYPE\n");
  for (struct memory_descriptor *ent = mmap; (u8 *)ent < (u8 *)mmap + mmap_sz; ent = NEXT_DESC(ent, desc_sz)) {
    switch (ent->type) {
      case 1:
      case 2:
      case 3:
      case 4:
      case 7:
        printf("%lx %lx %x MEMORY (%hhx)\n", ent->physical_addr, ent->virtual_addr, ent->n_pages, ent->type);
        break;
      case 9:
        printf("%lx %lx %x RECLAIM\n", ent->physical_addr, ent->virtual_addr, ent->n_pages);
        break;
      case 10:
        printf("%lx %lx %x NVS\n", ent->physical_addr, ent->virtual_addr, ent->n_pages);
        break;
      case 14:
        printf("%lx %lx %x PERSISTENT MEMORY\n", ent->physical_addr, ent->virtual_addr, ent->n_pages);
        break;
      default:
        printf("%lx %lx %x RESERVED\n", ent->physical_addr, ent->virtual_addr, ent->n_pages);
        break;
    }
  }
  return 0;
}


