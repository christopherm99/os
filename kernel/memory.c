#include <memory/init.h>
#include <types.h>
#include <io.h>
#include <math.h>


struct {
  u8    max_depth;
  void *heap;
} *kmem = NULL;

// Start having the kernel manage memory
void kmem_init(void *rbp, struct memory_descriptor *mmap, u64 mmap_sz, u64 desc_sz) {
  printf("STACK BEGINS AT: %lx\n", rbp);

  u64 extent = 0;
  u64 tot = 0;
  printf("ADDRESS BEGIN    ADDRESS END      TYPE\n");
  for (struct memory_descriptor *ent = mmap; (u8 *)ent < (u8 *)mmap + mmap_sz; ent = NEXT_DESC(ent, desc_sz)) {
    switch (ent->type) {
      case 1:
        printf("%lx %lx KERNEL CODE\n", ent->physical_addr, ent->physical_addr + ent->n_pages * 4096);
        break;
      case 2:
        printf("%lx %lx KERNEL DATA\n", ent->physical_addr, ent->physical_addr + ent->n_pages * 4096);
        break;
      case 3:
      case 4:
      case 7:
        // printf("%lx %lx MEMORY (%hhx)\n", ent->physical_addr, ent->physical_addr + ent->n_pages * 4096, ent->type);
        u64 sz = ent->physical_addr + ent->n_pages * 4096;
        tot += sz;
        if (sz > extent) extent = sz;
        break;
      case 9:
        printf("%lx %lx RECLAIM\n", ent->physical_addr, ent->physical_addr + ent->n_pages * 4096);
        break;
      case 10:
        printf("%lx %lx NVS\n", ent->physical_addr, ent->physical_addr + ent->n_pages * 4096);
        break;
      case 14:
        printf("%lx %lx PERSISTENT MEMORY\n", ent->physical_addr, ent->physical_addr + ent->n_pages * 4096);
        break;
      default:
        printf("%lx %lx RESERVED (%d)\n", ent->physical_addr, ent->physical_addr + ent->n_pages * 4096, ent->type);
        break;
    }
  }
  printf("TOTAL ADDRESS SPACE: 0x%x MAX ORDER: %d\n", extent, ceil_log2(extent / 4096));
  printf("HEAP SIZE: 0x%x bytes\n", extent / 2048 - 1);
  printf("TOTAL USABLE MEMORY: 0x%x bytes\n", tot);
  __asm("int $0xFF");
}

