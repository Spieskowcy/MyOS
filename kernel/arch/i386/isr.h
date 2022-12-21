#ifndef _ISR_H_
#define _ISR_H_

#include "isr_arch.h"
#include "proc.h"
#include <stddef.h>

void isr_handler(struct isr_frame *ifr);

typedef void (*isr_handler_t)(void);

void isr_register_handler(unsigned int num, isr_handler_t func);

void isr_init(void);

#endif 



