#include <stdio.h>

#include <kernel/keyboard.h>
#include <kernel/com.h>
#include <kernel/asm.h>
#include <test.h>

#include <string.h>


TEST(strlenTest){
	char* str = "abc";
	size_t size = strlen(str);
	ASSERT(size, 3);
}

TEST(scanCodessmallATest){
	unsigned char a = 0x1C;
	char result = keyboard_decode(a);
	ASSERT(result, 'a');
}

void kernel_main(void) {
	serial_initialize();

	RUN(strlenTest);
	RUN(scanCodessmallATest);

	outb(0xf4, 0x00); // shutdown
}
