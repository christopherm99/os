#ifndef _IO_H
#define _IO_H
#include <types.h>

void write(const char *data, u32 size);
void print(const char *str);
void printf(const char *fmt, ...);

inline void outb(u16 port, u8 val) {
  __asm volatile ("outb %b0, %w1" :: "a"(val), "Nd"(port) : "memory");
}

inline u8 inb(u16 port) {
  u8 ret;
  __asm volatile ("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

#endif

