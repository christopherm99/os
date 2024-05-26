#include <memory/init.h>
#include <memory/paging.h>
#include <types.h>
#include <io.h>
#include <math.h>

struct {
  u8    max_depth;
  void *heap;
} *kmem = NULL;

// We are using 4K pages
void paging_init(void *ptr, u64 max_addr) {
  u64 num_pdpt = (max_addr >> 39) + !!(max_addr & 0x7FFFFFFFFF);
  u64 num_pd   = (max_addr >> 30) + !!(max_addr &   0x3FFFFFFF);
  u64 num_pt   = (max_addr >> 21) + !!(max_addr &     0x1FFFFF);
  u64 num_p    = (max_addr >> 12) + !!(max_addr &       0x0FFF);
  printf("Allocating %d tables for PAGEDATA\n", 1 + num_pdpt + num_pd + num_pt);
  table_t PML4 = (table_t)ptr;
  ptr = (void *)((char *)ptr + 4096);
  table_t *PDPTs = (table_t *)ptr;
  ptr = (void *)((char *)ptr + 4096 * num_pdpt);
  table_t *PDs = (table_t *)ptr;
  ptr = (void *)((char *)ptr + 4096 * num_pd);
  table_t *PTs = (table_t *)ptr;
  ptr = (void *)((char *)ptr + 4096 * num_pt);
  printf("PML4:  %lx (%lx)\n", PML4, &PML4[0]);
  printf("PDPTs: %lx (%lx)\n", PDPTs, &(PDPTs[0]));
  printf("PDs:   %lx\n", PDs);
  printf("PTs:   %lx\n", PTs);
  __asm("int $0xFF");
  printf("Creating %d pages\n", num_p);
  for (u64 page = 0; page < num_p; page++) {
    u64 table = page / 512;
    table_entry_s pte = PTs[table][page % 512];
    pte.p    = 1;
    pte.r_w  = 1;
    pte.addr = page;
    if (page % 512 == 0) {
      u64 dir = table / 512;
      table_entry_s pde = PDs[dir][table % 512];
      pde.p    = 1;
      pde.r_w  = 1;
      pde.addr = (u64)(&pte) >> 12;
      printf("PDE:   0x%lx (dir=%d,table mod 512=%d)\n", &pde, dir, table % 512);
      if (table % 512 == 0) {
        u64 dir_ptr = dir / 512;
        table_entry_s pdpte = PDPTs[dir_ptr][dir % 512];
        pdpte.p    = 1;
        pdpte.r_w  = 1;
        pdpte.addr = (u64)(&pde) >> 12;
        printf("PDPTE: 0x%lx\n", &pdpte);
        if (dir % 512 == 0) {
          table_entry_s pml4e = PML4[dir_ptr % 512];
          pml4e.p    = 1;
          pml4e.r_w  = 1;
          pml4e.addr = (u64)(&pdpte) >> 12;
          printf("PML4E: 0x%lx (dptr mod 512=%d)\n", &pml4e, dir_ptr);
        }
      }
    }
  }
}

// Start having the kernel manage memory
// STACK: MEMORY (4)
// PAGE TABLES: MEMORY {4}
void kmem_init(void *rbp, struct memory_descriptor *mmap, u64 mmap_sz, u64 desc_sz) {
  printf("STACK BEGINS AT: %lx\n", rbp);
  u64 extent = 0;
  u64 tot = 0;
  struct memory_descriptor *largest = NULL;
  printf("ADDRESS BEGIN    ADDRESS END      TYPE\n");
  for (struct memory_descriptor *ent = mmap; (u8 *)ent < (u8 *)mmap + mmap_sz; ent = NEXT_DESC(ent, desc_sz)) {
    switch (ent->type) {
      case 3:
      case 4:
      case 7:
        // printf("%lx %lx MEMORY (%hhx)\n", ent->physical_addr, ent->physical_addr + ent->n_pages * 4096, ent->type);
        u64 sz = ent->n_pages * 4096;
        tot += sz;
        if (ent->physical_addr + sz > extent) extent = ent->physical_addr + sz;
        if (largest == NULL || ent->n_pages > largest->n_pages) largest = ent;
        break;
      case 1:
        printf("%lx %lx KERNEL CODE\n", ent->physical_addr, ent->physical_addr + ent->n_pages * 4096);
        break;
      case 2:
        printf("%lx %lx KERNEL DATA\n", ent->physical_addr, ent->physical_addr + ent->n_pages * 4096);
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
  printf("ADDRESS SPACE: 0x%lx MAX ORDER: %d\n", extent, ceil_log2(extent / 4096));
  printf("USABLE MEMORY: 0x%lx bytes\n", tot);
  printf("LARGEST SPACE: 0x%lx (%d KiB)\n", largest->physical_addr, largest->n_pages * 4);
  paging_init((void *)largest->physical_addr, extent);

  __asm("int $0xFF");
}

