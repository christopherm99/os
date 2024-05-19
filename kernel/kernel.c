#include <io.h>
#include <kalloc/memory.h>
#include <kalloc/kalloc.h>

void kmain(void *mmap, u64 mmap_sz, u64 desc_sz) {
  kalloc_init((struct memory_descriptor *)mmap, mmap_sz, desc_sz);
  while (true) {};
}

