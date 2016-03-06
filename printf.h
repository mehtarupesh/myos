#ifndef __PRINTF_H__
#define __PRINTF_H__

#include <stdarg.h>

void printf(char *fmt, ...);
void vprintf(char *fmt, va_list va);

#endif