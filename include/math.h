#ifndef _MATH_H
#define _MATH_H
#include <types.h>

#define CEIL_DIV(x, y) ((x) + (y) - 1) / (y)

static inline u64 log2(u64 v) {
  return 8 * sizeof(u64) - __builtin_clzll(v) - 1;
}

static inline u64 ceil_log2(u64 v) {
  return 8 * sizeof(u64) - __builtin_clzll(v) - (__builtin_popcount(v) == 1);
}

#endif

