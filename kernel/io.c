#include <io.h>
#include <arg.h>
#include <types.h>
#include <string.h>

#define COM1 0x3F8
void serial_init() {
  outb(COM1 + 1, 0x00); // Disable interrupts
  outb(COM1 + 3, 0x80); // Enable DLAB
  outb(COM1 + 0, 0x03); // Set divisor (baud rate)
  outb(COM1 + 1, 0x00);
  outb(COM1 + 3, 0x03); // 8 bits, one stop bit, no parity
  outb(COM1 + 2, 0xC7); // enable and clear FIFO, interrupt on 14 bits available
  outb(COM1 + 4, 0x0F); // setup modem control register
  // NOTE: We skip testing via loopback
}

int is_transmit_empty() {
  return inb(COM1 + 5) & 0x20;
}

void putchar(char a) {
  if (a == '\n') {
    while (is_transmit_empty() == 0) {};
    outb(COM1, '\r');
    while (is_transmit_empty() == 0) {};
    outb(COM1, '\n');
  } else {
    while (is_transmit_empty() == 0) {};
    outb(COM1, a);
  }
}

void write(const char* data, u32 size) {
  for (u32 i = 0; i < size; i++) putchar(data[i]);
}

void print(const char* str) {
  write(str, strlen(str));
}

// TODO: there has to be a better way to do this
static char *digit = "0123456789ABCDEF";

void printu64(u64 data) {
  for (int i = 60; i >= 0; i -= 4)
    putchar(digit[data >> i & 0xF]);
}

void printu32(u32 data) {
  for (int i = 28; i >= 0; i -= 4)
    putchar(digit[data >> i & 0xF]);
}

void printu16(u16 data) {
  for (int i = 12; i >= 0; i -= 4)
    putchar(digit[data >> i & 0xF]);
}

void printu8(u8 data) {
  // TODO: are these for loops being unrolled?
  for (int i = 4; i >= 0; i -= 4)
    putchar(digit[data >> i & 0xF]);
}

void printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  for (u32 i = 0; fmt[i] != '\0';) {
    if (fmt[i] == '%') {
      switch (fmt[++i]) {
        case 'x':
          printu32(va_arg(ap, int));
          break;
        case 'l':
          if (fmt[++i] == 'x') {
            printu64(va_arg(ap, long long int));
            break;
          }
          goto fail;
        case 'h':
          if (fmt[i+1] == 'h' && fmt[++i + 1] == 'x') {
            i++;
            printu8(va_arg(ap, int));
            break;
          } else if (fmt[++i] == 'x') {
            printu16(va_arg(ap, int));
            break;
          }
          /* FALLTHROUGH */
        default:
fail:     putchar('%'); putchar(fmt[i+1]);
          break;
      }
      i++;
    } else putchar(fmt[i++]);
  }
  va_end(ap);
}

