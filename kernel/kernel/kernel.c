#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	int v = 12;
	int a = 0xF, b = 0xF0000000;
	int* wsk = &v; 
	printf("Are you working? %p wsk: %p  hex: %X  hex2: %x dec: %d dec2: %i\n", &v, wsk, b, a, b, a);
}
