#include <io.h>

void kmain() {
  // print("hello!\n");
  __asm volatile ("int $0xFF");
}
