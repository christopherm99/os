#ifndef _INTERRUPT_H
#define _INTERRUPT_H
#include <types.h>

#define INT_DOUBLE_FAULT 0x00000002
#define INT_GENERAL_PROTECTION 0x0000000D
#define INT_PAGE_FAULT 0x0000000E
#define INT_MACHINE_CHECK 0x00000012
#define INT_SYSCALL 0x00000080

struct cpu_state {
  u32 eax;
  u32 ebx;
  u32 ecx;
  u32 edx;
  u32 esi;
  u32 edi;
  u32 ebp;
} __attribute__((packed));

u32 interrupt_handler(struct cpu_state cpu, u32 vector, u32 error, u32 eip);

#endif

