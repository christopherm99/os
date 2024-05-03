#ifndef _ARG_H
#define _ARG_H

#define va_list __builtin_va_list
#define va_start(a, b) __builtin_va_start((a), (b))
#define va_arg(a, b) __builtin_va_arg((a), b)
#define va_end(a) __builtin_va_end((a))

#endif

