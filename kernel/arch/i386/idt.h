#ifndef _IDT_H_
#define _IDT_H_

#include <stdint.h>
#include "pic.h"

#define IDT_MAX_DESCRIPTORS 256


struct idt_entry {
    uint16_t offset_lo;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t offset_hi;
};

struct idt_register {
    uint16_t limit;
    uint16_t base_lo;
    uint16_t base_hi;
};

static void idt_entry_init(unsigned int, uint32_t, uint8_t, uint16_t);

__attribute__((noreturn))
void exception_handler(void);

void idt_init(void);


#endif // !_IDT_H_
