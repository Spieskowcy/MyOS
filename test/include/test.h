#ifndef TEST_TEST_H
#define TEST_TEST_H

#include <kernel/com.h>

#define FAIL {serial_writestring("Test terminated\n"); return -2;}
#define PASS {serial_writestring("Test passed\n"); return 0;}
#define RETURN return error_count;

void print_bad_assertion(int x, int y){
		serial_writestring("Expected "); // TEMP to be replaced with sprintf
}

int assert(int x, int y){
	return x == y;
}

#define ASSERT(x, y) if (x == y) {} else { print_bad_assertion(x,y); return -1;}
#define EXPECT(x, y) if (x == y) {} else { print_bad_assertion(x,y); error_count++;}


#define TEST(x) int x##T(int); int x() { return x##T(0);} int x##T(int error_count)

#define RUN(x) serial_writestring("Started: "); serial_writestring(#x); serial_writestring("\n"); \
	if(x() == 0) { serial_writestring(#x); serial_writestring("[PASS]\n");}\ 
 else {serial_writestring(#x); serial_writestring("[FAIL]\n");}\



#endif
