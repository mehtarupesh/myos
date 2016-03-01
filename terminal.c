#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
#include "string.h"

static const uint8_t COLOR_BLACK = 0;
static const uint8_t COLOR_BLUE = 1;
static const uint8_t COLOR_GREEN = 2;
static const uint8_t COLOR_CYAN = 3;
static const uint8_t COLOR_RED = 4;
static const uint8_t COLOR_MAGENTA = 5;
static const uint8_t COLOR_BROWN = 6;
static const uint8_t COLOR_LIGHT_GREY = 7;
static const uint8_t COLOR_DARK_GREY = 8;
static const uint8_t COLOR_LIGHT_BLUE = 9;
static const uint8_t COLOR_LIGHT_GREEN = 10;
static const uint8_t COLOR_LIGHT_CYAN = 11;
static const uint8_t COLOR_LIGHT_RED = 12;
static const uint8_t COLOR_LIGHT_MAGENTA = 13;
static const uint8_t COLOR_LIGHT_BROWN = 14;
static const uint8_t COLOR_WHITE = 15;

static uint8_t make_color(uint8_t fg, uint8_t bg)
{
	return fg | bg << 4;
}

static uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 24;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_initialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
}

static void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

static void terminal_putentryat(uint16_t vga_entry, size_t col, size_t row)
{
	const size_t index = row * VGA_WIDTH + col;
	terminal_buffer[index] = vga_entry;
}

static uint16_t terminal_getentryat(size_t col, size_t row)
{
	const size_t index = row * VGA_WIDTH + col;
	return terminal_buffer[index];
}

static void terminal_clear_row_from(size_t col, size_t row)
{
    size_t i;
    uint16_t vga_entry = make_vgaentry(' ', terminal_color);
    for(i = col; i < VGA_WIDTH; i++)
        terminal_putentryat(vga_entry, i, row);
}

static void terminal_scroll(void)
{

    /* need to move all rows up by one index
       losing information about row 0 */
   size_t temp_row, temp_column;
    /* for each row */
     for(temp_row = 1; temp_row < VGA_HEIGHT; temp_row++) {

        for(temp_column = 0; temp_column < VGA_WIDTH; temp_column++){
            uint16_t ventry = terminal_getentryat(temp_column, temp_row);
            terminal_putentryat(ventry, temp_column, temp_row - 1);
        }
    }

    /* clear the last row, making 'space' */
    terminal_clear_row_from(0, VGA_HEIGHT - 1);

}

void terminal_putchar(char c)
{

    /* handle newline char */
    if (c == '\n') {

        terminal_clear_row_from(terminal_column, terminal_row);
		terminal_column = 0;
        
        if( ++terminal_row == VGA_HEIGHT) {
			terminal_scroll();
			terminal_row--;
        }
        return;
    }

	uint16_t vga_entry = make_vgaentry(c, terminal_color);
	terminal_putentryat(vga_entry, terminal_column, terminal_row);

	/* handle moving to next line */
	if ( ++terminal_column == VGA_WIDTH )
	{
		terminal_column = 0;
		if ( ++terminal_row == VGA_HEIGHT )
		{
			terminal_scroll();
			terminal_row--;
		}
	}
}

void terminal_writestring(const char* data)
{
	size_t datalen = strlen(data);
	for ( size_t i = 0; i < datalen; i++ )
		terminal_putchar(data[i]);
}

static char* test_string="NEWSTRING\n";
void terminal_test() {

    size_t i;
    terminal_writestring(test_string);
    for(i = 0; i < VGA_HEIGHT; i++) {
        terminal_writestring("Hello Mehta\n");
    }
    terminal_writestring(test_string);
}
