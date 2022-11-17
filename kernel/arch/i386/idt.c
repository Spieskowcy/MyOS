#include "idt.h"

void isr_0(void);
void isr_1(void);
void isr_2(void);
void isr_3(void);
void isr_4(void);
void isr_5(void);
void isr_6(void);
void isr_7(void);
void isr_8(void);
void isr_9(void);
void isr_10(void);
void isr_11(void);
void isr_12(void);
void isr_13(void);
void isr_14(void);
void isr_15(void);
void isr_16(void);
void isr_17(void);
void isr_18(void);
void isr_19(void);
void isr_20(void);
void isr_21(void);
void isr_22(void);
void isr_23(void);
void isr_24(void);
void isr_25(void);
void isr_26(void);
void isr_27(void);
void isr_28(void);
void isr_29(void);
void isr_30(void);
void isr_31(void);
void isr_32(void);
void isr_33(void);
void isr_34(void);
void isr_35(void);
void isr_36(void);
void isr_37(void);
void isr_38(void);
void isr_39(void);
void isr_40(void);
void isr_41(void);
void isr_42(void);
void isr_43(void);
void isr_44(void);
void isr_45(void);
void isr_46(void);
void isr_47(void);
void isr_128(void);

static struct idt_entry     idt_entries[IDT_MAX_DESCRIPTORS];
static struct idt_register  idt_reg;

static void idt_entry_init(unsigned int i, uint32_t offset, uint8_t flags, uint16_t selector)
{
    idt_entries[i].zero = 0;
    idt_entries[i].offset_lo  = offset & 0x0000FFFF;
    idt_entries[i].offset_hi  = offset >> 16;
    idt_entries[i].selector = selector;
    idt_entries[i].flags = flags;
}

void exception_handler() {
    __asm__ volatile ("cli; hlt"); 
}

void idt_init(void)
{
    uint32_t idt_addr = (uint32_t)&idt_entries;

    idt_reg.limit = sizeof(struct idt_entry) * IDT_MAX_DESCRIPTORS - 1;
    idt_reg.base_lo = idt_addr & 0xFFFF;
    idt_reg.base_hi = (idt_addr >> 16) & 0xFFFF;

    idt_entry_init(0,  (uint32_t) isr_0,  0x8E, 0x08);
    idt_entry_init(1,  (uint32_t) isr_1,  0x8E, 0x08);
    idt_entry_init(2,  (uint32_t) isr_2,  0x8E, 0x08);
    idt_entry_init(3,  (uint32_t) isr_3,  0x8E, 0x08);
    idt_entry_init(4,  (uint32_t) isr_4,  0x8E, 0x08);
    idt_entry_init(5,  (uint32_t) isr_5,  0x8E, 0x08);
    idt_entry_init(6,  (uint32_t) isr_6,  0x8E, 0x08);
    idt_entry_init(7,  (uint32_t) isr_7,  0x8E, 0x08);
    idt_entry_init(8,  (uint32_t) isr_8,  0x8E, 0x08);
    idt_entry_init(9,  (uint32_t) isr_9,  0x8E, 0x08);
    idt_entry_init(10, (uint32_t) isr_10, 0x8E, 0x08);
    idt_entry_init(11, (uint32_t) isr_11, 0x8E, 0x08);
    idt_entry_init(12, (uint32_t) isr_12, 0x8E, 0x08);
    idt_entry_init(13, (uint32_t) isr_13, 0x8E, 0x08);
    idt_entry_init(14, (uint32_t) isr_14, 0x8E, 0x08);
    idt_entry_init(15, (uint32_t) isr_15, 0x8E, 0x08);
    idt_entry_init(16, (uint32_t) isr_16, 0x8E, 0x08);
    idt_entry_init(17, (uint32_t) isr_17, 0x8E, 0x08);
    idt_entry_init(18, (uint32_t) isr_18, 0x8E, 0x08);
    idt_entry_init(19, (uint32_t) isr_19, 0x8E, 0x08);
    idt_entry_init(20, (uint32_t) isr_20, 0x8E, 0x08);
    idt_entry_init(21, (uint32_t) isr_21, 0x8E, 0x08);
    idt_entry_init(22, (uint32_t) isr_22, 0x8E, 0x08);
    idt_entry_init(23, (uint32_t) isr_23, 0x8E, 0x08);
    idt_entry_init(24, (uint32_t) isr_24, 0x8E, 0x08);
    idt_entry_init(25, (uint32_t) isr_25, 0x8E, 0x08);
    idt_entry_init(26, (uint32_t) isr_26, 0x8E, 0x08);
    idt_entry_init(27, (uint32_t) isr_27, 0x8E, 0x08);
    idt_entry_init(28, (uint32_t) isr_28, 0x8E, 0x08);
    idt_entry_init(29, (uint32_t) isr_29, 0x8E, 0x08);
    idt_entry_init(30, (uint32_t) isr_30, 0x8E, 0x08);
    idt_entry_init(31, (uint32_t) isr_31, 0x8E, 0x08);

    idt_entry_init(32, (uint32_t) isr_32, 0x8E, 0x08);
    idt_entry_init(33, (uint32_t) isr_33, 0x8E, 0x08);
    idt_entry_init(34, (uint32_t) isr_34, 0x8E, 0x08);
    idt_entry_init(35, (uint32_t) isr_35, 0x8E, 0x08);
    idt_entry_init(36, (uint32_t) isr_36, 0x8E, 0x08);
    idt_entry_init(37, (uint32_t) isr_37, 0x8E, 0x08);
    idt_entry_init(38, (uint32_t) isr_38, 0x8E, 0x08);
    idt_entry_init(39, (uint32_t) isr_39, 0x8E, 0x08);
    idt_entry_init(40, (uint32_t) isr_40, 0x8E, 0x08);
    idt_entry_init(41, (uint32_t) isr_41, 0x8E, 0x08);
    idt_entry_init(42, (uint32_t) isr_42, 0x8E, 0x08);
    idt_entry_init(43, (uint32_t) isr_43, 0x8E, 0x08);
    idt_entry_init(44, (uint32_t) isr_44, 0x8E, 0x08);
    idt_entry_init(45, (uint32_t) isr_45, 0x8E, 0x08);
    idt_entry_init(46, (uint32_t) isr_46, 0x8E, 0x08);
    idt_entry_init(47, (uint32_t) isr_47, 0x8E, 0x08);

    idt_entry_init(128, (uint32_t) isr_128, 0xEE, 0x08);


    __asm__ volatile ("lidt %0" : : "m"(idt_reg));
	__asm__ volatile ("sti");
}

