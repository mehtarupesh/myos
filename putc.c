#include <putc.h>
#include <terminal.h>

void putc(char ch) {
	terminal_putchar(ch);
}
