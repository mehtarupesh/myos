#include "printf.h"
#include "putc.h"
#include "string.h"

void printf(char *fmt, ...) {

	int i;
	int len = strlen(fmt);

	for(i = 0; i < len; i++) {
		putc(fmt[i]);
	}	

}

void vprintf(char *fmt, va_list va) {

}
