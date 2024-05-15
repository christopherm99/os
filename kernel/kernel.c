#include <io.h>

void kmain() {
  __asm volatile ("int $0xFF");
  while (true) {};
}

