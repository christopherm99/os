#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t trow;
static size_t tcol;
static uint8_t tcolor;
static uint16_t* tbuffer;

void terminal_init() {
  trow = 0;
  tcol = 0;
  tcolor = 0xF; // wob
  for (size_t i = 0; i < VGA_HEIGHT; i++)
    for (size_t j = 0; i < VGA_WIDTH; j++)
      tbuffer[i * VGA_WIDTH + j] = ' ' | tcolor << 8;
}

void terminal_putchar(char c) {
  if (!init) vga_init();
  if (c == '\n') { tcol = 0; trow++; }
  else tbuffer[trow*80 + (tcol++)] = (uint16_t)c | tcolor << 8;
  if (tcol == 80) { tcol = 0; trow++; }
  if (trow == 25) {
    memmove(tbuffer, tbuffer + 80, 24 * 80 * 2);
    for (size_t i = 0; i < 80; i++) tbuffer[24*80 + i] = (uint16_t)' ' | tcolor << 8;
    trow = 24;
  }
}

void terminal_write(const char* data, size_t size) {
  for (size_t i = 0; i < size; i++) terminal_putchar(data[i]);
}

void terminal_print(const char* str) {
  terminal_write(str, strlen(str));
}

void terminal_printnum(int data) {
  char* digit = "0123456789ABCDEF";
  for (int i = 0; i < 16; i += 4)
    terminal_putchar(digit[data >> i & 0xF]);
}


void terminal_printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  for (size_t i = 0; fmt[i] != '\0'; i++) {
    if (fmt[i] == '%') {
      switch (fmt[i+1]) {
        case 'x':
          terminal_printnum(va_arg(ap, int));
          break;
        default:
          terminal_putchar('%'); terminal_putchar(fmt[i+1]);
          break;
      }
      i += 2;
    } else terminal_putchar(fmt[i++]);
  }
  va_end(ap);
}


