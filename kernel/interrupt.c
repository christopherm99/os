#include <interrupt.h>
#include <io.h>

u64 interrupt_handler(struct cpu_state cpu, u64 vector, u64 error, u64 rip) {
  printf("INTERRUPT: vector=%hhx error=%lx\n", vector, error);
  printf("RAX %lx RBX %lx RCX %lx RDX %lx\n", cpu.rax, cpu.rbx, cpu.rcx, cpu.rdx);
  printf("RSI %lx RDI %lx RBP %lx RIP %lx\n", cpu.rsi, cpu.rdi, cpu.rbp, rip);
  printf("R8  %lx R9  %lx R10 %lx R11 %lx\n", cpu.r8,  cpu.r9,  cpu.r10, cpu.r11);
  printf("R12 %lx R13 %lx R14 %lx R15 %lx\n", cpu.r12, cpu.r13, cpu.r14, cpu.r15);
  while (true) {}
  return 0;
}

