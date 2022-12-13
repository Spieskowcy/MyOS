.macro ISR num
    .globl isr_\num
    isr_\num:
        call exception_handler
    	iret
.endm


.macro ISR_ERR num
    .globl isr_\num
    isr_\num:
   	call exception_handler
    	iret
.endm

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR_ERR 8
ISR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31
ISR 32
ISR 33
ISR 34
ISR 35
ISR 36
ISR 37
ISR 38
ISR 39
ISR 40
ISR 41
ISR 42
ISR 43
ISR 44
ISR 45
ISR 46
ISR 47
ISR 128

isr_common:
    pusha               /* Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax */
    mov     %ds, %ax    /* Get the original data segment descriptor */
    push    %eax        /* And save it into the stack  */
    mov     $0x10, %ax  /* Load the kernel data segment descriptor */
    mov     %ax, %ds
    mov     %ax, %es
    mov     %ax, %fs
    mov     %ax, %gs
    push    %esp        /* Push a pointer to an isr_frame struct */
    call    isr_handler /* Call the arch independent dispatcher */
    add     $4, %esp
.global fork_ret
fork_ret:
    pop     %eax        /* Reload the original data segment descriptor */
    mov     %ax, %ds
    mov     %ax, %es
    mov     %ax, %fs
    mov     %ax, %gs
    popa                /* Pop edi,esi,ebp,esp,ebx,edx,ecx,eax */
    add     $8, %esp    /* Clean up the pushed error code and isr number */
    iret                /* pops 5 things at once: cs,eip,eflags,ss,esp */

/*
 * Send the EOI (end of interrupt) to the PIC
 */
.global pic_eoi
pic_eoi:
    mov     $0x20, %eax     /* command */
    mov     4(%esp), %ecx
    cmp     $39, %ecx
    jle     1f
    mov     $0xA0, %edx     /* slave port */
    outb    %al, %dx
1:  mov     $0x20, %edx     /* master port */
    outb    %al, %dx
    ret


