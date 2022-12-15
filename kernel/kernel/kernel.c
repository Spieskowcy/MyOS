#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/keyboard.h>

void kernel_main(void) {
	terminal_initialize();
	int v = 12;
	int a = 0xF, b = 0xF0000000;
	int* wsk = &v; 
	printf("Are you working? %p wsk: %p  hex: %X  hex2: %x dec: %d dec2: %i\n", &v, wsk, b, a, b, a);

	char* keyboard_address= 0x041E;
	char text[2];

	for(int i=0; i<1000; i++)
	{
		text[1] = '\0';
		text[0] = *keyboard_address;
		printf(text);
		*keyboard_address = '\0';
	}
}
