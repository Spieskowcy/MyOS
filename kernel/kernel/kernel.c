#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	int v = 12;
	printf("Are you working? %p\n", &v);
}
