#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/keyboard.h>

#include "vga.h"

static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void enable_cursor(unsigned char cursor_start, unsigned char cursor_end)
{
	outportb(0x3D4, 0x0A);
	outportb(0x3D5, (inportb(0x3D5) & 0xC0) | cursor_start);
 
	outportb(0x3D4, 0x0B);
	outportb(0x3D5, (inportb(0x3D5) & 0xE0) | cursor_end);
}
void update_cursor(int x, int y)
{
	unsigned int pos = y * VGA_WIDTH + x;
 
	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (unsigned char) (pos & 0xFF));
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (unsigned char) ((pos >> 8) & 0xFF));
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	enable_cursor(0, 15);
	update_cursor(terminal_column, terminal_row);
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
	unsigned char uc = c;
	if(uc =='\n')
	{
		terminal_column = 0;
		terminal_row++;
		update_cursor(terminal_column, terminal_row);
		return;
	}
	if(uc == 0x08)
	{
		const size_t index = terminal_row * VGA_WIDTH + terminal_column;
		terminal_buffer[index] = vga_entry(' ', terminal_color);
		if(terminal_column!=0)
			terminal_column--;
		update_cursor(terminal_column, terminal_row);
		return;
	}
	terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
	update_cursor(terminal_column, terminal_row);
}

int terminal_getstring(char text[]) {
	int length = 0;
	for(int i=0; i<terminal_column; i++)
	{
		text[i] = VGA_MEMORY[terminal_row*VGA_WIDTH + i];
		length++;
	}
	text[length] = '\0';
	return terminal_column+1;
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
