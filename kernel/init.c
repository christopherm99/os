#include <init.h>

#include <io.h>
#include <memory/init.h>

void kinit(void *mmap, u64 mmap_sz, u64 desc_sz) {

  kmem_init(__builtin_frame_address(0), (struct memory_descriptor *)mmap, mmap_sz, desc_sz);
  while (true) {};
}

