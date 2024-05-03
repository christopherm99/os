#ifndef _STRING_H
#define _STRING_H
#include <types.h>

void* memmove(void *dstptr, const void *srcptr, u32 n);

u32 strlen(const char *str);

#endif

