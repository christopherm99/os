#ifndef _INTERRUPT_H
#define _INTERRUPT_H
#include <types.h>

#define INT_DOUBLE_FAULT 0x00000002
#define INT_GENERAL_PROTECTION 0x0000000D
#define INT_PAGE_FAULT 0x0000000E
#define INT_MACHINE_CHECK 0x00000012
#define INT_SYSCALL 0x00000080

struct cpu_state {
  u64 rax;
  u64 rbx;
  u64 rcx;
  u64 rdx;
  u64 rbp;
  u64 rsi;
  u64 rdi;
  u64 r8;
  u64 r9;
  u64 r10;
  u64 r11;
  u64 r12;
  u64 r13;
  u64 r14;
  u64 r15;
} __attribute__((packed));

u64 interrupt_handler(struct cpu_state cpu, u64 vector, u64 error, u64 rip);

#endif

