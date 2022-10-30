#ifndef _KERNEL_COM_H
#define _KERNEL_COM_H

#include <stddef.h>
#include <kernel/asm.h>
#define PORT 0x3f8          // COM1
void serial_initialize(void);
void serial_putchar(char c);
void serial_write(const char* data, size_t size);
void serial_writestring(const char* data);

#endif
