#include <stdio.h>

#include <kernel/com.h>
#include <kernel/asm.h>
#include <test.h>

#include <string.h>


TEST(strlenTest){
	char* str = "abc";
	size_t size = strlen(str);
	ASSERT(size, 3);
}
void kernel_main(void) {
	serial_initialize();

	RUN(strlenTest);

	outb(0xf4, 0x00); // shutdown
}
