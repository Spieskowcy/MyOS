#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
int terminal_getstring(char text[]);
void enable_cursor(unsigned char cursor_start, unsigned char cursor_end);
void update_cursor(int x, int y);

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
#endif

