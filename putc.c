#include <putc.h>
#include <terminal.h>
#include <stddef.h>

void putc(char ch) {
	terminal_putchar(ch);
}

void puts(char *string) {

	if(string == NULL)
		string = "NULL";

	while(*string)
		terminal_putchar(*string++);

}
