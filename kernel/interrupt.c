#include <interrupt.h>
#include <io.h>

u32 interrupt_handler(struct cpu_state cpu, u32 vector, u32 error, u32 eip) {
  printf("INTERRUPT: vector=%x error=%x\n", vector, error);
  printf("EAX %x EBX %x ECX %x EDX %x\n", cpu.eax, cpu.ebx, cpu.ecx, cpu.edx);
  printf("ESI %x EDI %x EBP %x EIP %x\n", cpu.esi, cpu.edi, cpu.ebp, eip);
  return 0;
}

