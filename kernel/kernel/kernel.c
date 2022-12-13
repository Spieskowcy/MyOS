#include <stdio.h>

#include <kernel/tty.h>
void func(void){
    printf("text");
}

void kernel_main(void) {
    isr_register_handler(0x81, func);
	terminal_initialize();
    __asm__ volatile ("int 81h");
	int v = 12;
	int a = 0xF, b = 0xF0000000;
	int* wsk = &v; 
	printf("Are you working? %p wsk: %p  hex: %X  hex2: %x dec: %d dec2: %i\n", &v, wsk, b, a, b, a);
}
