#include <io.h>
#include <arg.h>
#include <types.h>
#include <string.h>

bool init = false;
u32 trow;
u32 tcol;
u8 tcolor;
u16* tbuffer;

void vga_init() {
  trow = 0;
  tcol = 0;
  tcolor = 0xF; // white on black
  tbuffer = (u16 *) 0xB8000;
  for (u32 i = 0; i < 25; i++)
    for (u32 j = 0; j < 80; j++)
      tbuffer[i*80 + j] = (u16)' ' | tcolor << 8;
  init = true;
}


void putchar(char c) {
  if (!init) vga_init();
  if (c == '\n') { tcol = 0; trow++; }
  else tbuffer[trow*80 + (tcol++)] = (u16)c | tcolor << 8;
  if (tcol == 80) { tcol = 0; trow++; }
  if (trow == 25) {
    memmove(tbuffer, tbuffer + 80, 24 * 80 * 2);
    for (u32 i = 0; i < 80; i++) tbuffer[24*80 + i] = (u16)' ' | tcolor << 8;
    trow = 24;
  }
}

void write(const char* data, u32 size) {
  for (u32 i = 0; i < size; i++) putchar(data[i]);
}

void print(const char* str) {
  write(str, strlen(str));
}

void printnum(int data) {
  char* digit = "0123456789ABCDEF";
  for (int i = 0; i < 16; i += 4)
    putchar(digit[data >> i & 0xF]);
}


void printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  for (u32 i = 0; fmt[i] != '\0'; i++) {
    if (fmt[i] == '%') {
      switch (fmt[i+1]) {
        case 'x':
          printnum(va_arg(ap, int));
          break;
        default:
          putchar('%'); putchar(fmt[i+1]);
          break;
      }
      i += 2;
    } else putchar(fmt[i++]);
  }
  va_end(ap);
}


