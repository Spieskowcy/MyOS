#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	int v = 12;
	int *wsk = &v;
	printf("Are you working? %p wsk: %p\n", &v, wsk);
}
