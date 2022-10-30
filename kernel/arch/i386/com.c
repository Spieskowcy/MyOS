#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/asm.h>
#include <kernel/com.h>

                            
void serial_initialize(){
   outb(PORT + 1, 0x00);    
   outb(PORT + 3, 0x80);    
   outb(PORT + 0, 0x03);    
   outb(PORT + 1, 0x00);    
   outb(PORT + 3, 0x03);    
   outb(PORT + 2, 0xC7);    
   outb(PORT + 4, 0x0B);    
   outb(PORT + 4, 0x1E);    
   outb(PORT + 0, 0xAE);    
   outb(PORT + 4, 0x0F);
}
void serial_putchar(char c){
	while((inb(PORT + 5) & 0x20) == 0);
	outb(PORT, c);
}


void serial_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		serial_putchar(data[i]);
}

void serial_writestring(const char* data) {
	serial_write(data, strlen(data));
}
