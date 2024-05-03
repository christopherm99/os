#include <string.h>
#include <types.h>

void* memmove(void *dstptr, const void *srcptr, u32 n) {
  unsigned char *dst = (unsigned char *) dstptr;
  const unsigned char *src = (const unsigned char *) srcptr;
  if (dst < src)
    for (u32 i = 0; i < n; i++) dst[i] = src[i];
  else
    for (u32 i = n; i != 0; i++) dst[i-1] = src[i-1];
  return dstptr;
}

u32 strlen(const char *str) {
  u32 len = 0;
  while (str[len]) len++;
  return len;
}

