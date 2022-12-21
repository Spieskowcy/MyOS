#ifndef _ISR_ARCH_H_
#define _ISR_ARCH_H_

#include <stdint.h>

/*
 * Architecture-specific interrupt stack frame.
 * Some registers are automatically pushed by the processor while
 * others are pushed by the dispatching code for preservation or
 * for later reference.
 */
struct isr_frame {
    uint32_t    ds;         /* data segment selector */
    uint32_t    edi;        /* pushed by pusha */
    uint32_t    esi;        /* pushed by pusha */
    uint32_t    ebp;        /* pushed by pusha */
    uint32_t    esp;        /* pushed by pusha */
    uint32_t    ebx;        /* pushed by pusha */
    uint32_t    edx;        /* pushed by pusha */
    uint32_t    ecx;        /* pushed by pusha */
    uint32_t    eax;        /* pushed by pusha */
    uint32_t    int_no;     /* interrupt number */
    uint32_t    err_no;     /* error code */
    uint32_t    eip;        /* pushed by the processor */
    uint32_t    cs;         /* pushed by the processor */
    uint32_t    eflags;     /* pushed by the processor */
    uint32_t    usr_esp;    /* pushed by the processor */
    uint32_t    ss;         /* pushed by the processor */
};

/*
 * Interrupt numbers
 */
#define ISR_DIV_BY_ZERO 0
#define ISR_PAGE_FAULT  14
#define ISR_IRQ0        32
#define ISR_TIMER       32
#define ISR_KEYBOARD    33
#define ISR_COM2        35
#define ISR_COM1        36
#define ISR_SYSCALL     128

#endif
