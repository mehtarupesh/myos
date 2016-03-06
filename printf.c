#include <printf.h>
#include <putc.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

typedef unsigned int uint;

#define BUF_SIZE	32
#define VALID_DIGIT(x)	(x >= '0' && x <='9')

char digit[]="0123456789abcdef";

size_t printf_itoa(int value, uint radix, char *buf, uint zero_padding, int is_signed) {

	uint len = 0;

	if(is_signed && value < 0) {
		value = -value;
	}

	while(value) {

		uint rem = value % radix;

		buf[len] = digit[rem];

		value = value / radix;
		len++;
	}

	/* is this making sense ?*/
	if(len == BUF_SIZE)
		return;

	/* add padding */
	if(zero_padding > len)
		zero_padding = zero_padding - len;
	else
		zero_padding = 0;

	while(len < BUF_SIZE && zero_padding--) {
		buf[len] = '0';
		len++;
	}

	/* add sign */
	if(is_signed) {

		/* remove padding, make space */
		if(len == BUF_SIZE)
			len--;

		buf[len] = '-';
		len++;
	}

	/* reverse */
	uint iter;
	for(iter = 0; iter < len/2; iter++) {

		char temp = buf[iter];
		buf[iter] = buf[len - iter - 1];
		buf[len - iter - 1] = temp;
	}

	return len;

}

void vprintf(char *fmt, va_list va) {

	char buf[BUF_SIZE];
	size_t zero_padding = 0;
	size_t len = 0;
	size_t i = 0;
	char * s;

	if(fmt == NULL)
		return;

	while(*fmt) {

		char ch = *fmt;

		/* pass through non-formatted chars */
		if(ch != '%') {
			putc(ch);
			fmt++;
			continue;
		}

		/* get zero padding value */
		fmt++;

		if(fmt == NULL)
			return;

		ch = *fmt;

		if(VALID_DIGIT(ch)) {
			zero_padding = ch - '0';
			fmt++;
		}

		if(fmt == NULL)
			return;

		switch(ch = *fmt) {

			case 'd':
			case 'u':
					len = printf_itoa(va_arg(va, int), 10, buf, zero_padding, ch == 'd');

					i = 0;
					while(i < len) {
						putc(buf[i]);
						i++;
					}
					break;

			case 'x':
			case 'X':
					len = printf_itoa(va_arg(va, int), 16, buf, zero_padding, 0);

					i = 0;
					while(i < len) {
						putc(buf[i]);
						i++;
					}
					break;

			case 'c':
					putc((char)va_arg(va, int));
					break;

			case 's':
					s = va_arg(va, char*);
					puts(s);
					break;

			default:
					putc(ch);
					break;
		}

		fmt++;
	}
}

void test_printf() {

	printf("No arguments\n");
	printf("uint -1 = [%u]\n", -1);
	printf("int -1 = [%d]\n", -1);
	printf("char x = [%c]\n", 'x');
	printf("hex -1 = [%x]\n", -1);
	printf("formatted int -1 = [%8d]\n", -1);
	printf("formatted hex 45 = [%8x]\n", 45);
	printf("string hello world = [%s]\n", "hello world");

}

void printf(char *fmt, ...) {

	va_list va;
	va_start(va, fmt);
	vprintf(fmt, va);
	va_end(va);
}