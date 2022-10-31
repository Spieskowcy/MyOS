#ifndef TEST_TEST_H
#define TEST_TEST_H

#include <kernel/com.h>
#include <stdbool.h>

#define FAIL {serial_writestring("Test terminated\n"); *(error_count)++; return;}
#define PASS {serial_writestring("Test passed\n"); *(error_count) = 0; return;}

#define ASSERT(x, y) if (x == y) {} \
	else {serial_writestring("Assert fail on "); serial_writestring(__FILE__); /* serial_writestring(__LINE__); to be replaced with sprintf*/ (*error_count)++; return;}
#define EXPECT(x, y) if (x == y) {} \
	else { serial_writestring("Assert fail on "); serial_writestring(__FILE__); /* serial_writestring(__LINE__); to be replaced with sprintf*/ (*error_count)++;}

#define ASSERT_TRUE(x) ASSERT(x, true);
#define EXPECT_TRUE(x) EXPECT(x, true);

#define TEST(x) void x##T(int*); int x() { int ec = 0; x##T(&ec); return ec;} void x##T(int* error_count)

#define RUN(x) serial_writestring("Started: "); serial_writestring(#x); serial_writestring("\n"); \
	if(x() == 0) { serial_writestring(#x); serial_writestring("[PASS]\n");}\ 
	else {serial_writestring(#x); serial_writestring("[FAIL]\n");}\



#endif
