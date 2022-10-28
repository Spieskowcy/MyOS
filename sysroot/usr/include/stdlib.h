#ifndef _STDLIB_H
#define _STDLIB_H 0

#include <sys/cdefs.h>

#define DECIMAL 10

#ifdef __cplusplus
extern "C" {
#endif

int abs(int);
char* itoa(int, char*, int);

__attribute__((__noreturn__))
void abort(void);

#ifdef __cplusplus
}
#endif

#endif
