#include <pkaos.h>

/* --- 1. Lấy bảng từ ASM --- */
extern void *isr_stub_table[];
extern void *irq_stub_table[];

void isr_init() {
    interrupt_init();
}

/* --- 2. Khởi tạo interrupt --- */
void interrupt_init() {

    /* --- PIC Remap --- */
    outb(0x20, 0x11); outb(0xA0, 0x11);
    outb(0x21, 0x20); outb(0xA1, 0x28);
    outb(0x21, 0x04); outb(0xA1, 0x02);
    outb(0x21, 0x01); outb(0xA1, 0x01);
    outb(0x21, 0x0);  outb(0xA1, 0x0);

    /* --- Load ISR --- */
    for (int i = 0; i < 32; i++) {
        idt_set_gate(i, (uint32_t)isr_stub_table[i], 0x08, 0x8E);
    }

    /* --- Load IRQ --- */
    for (int i = 0; i < 16; i++) {
        idt_set_gate(i + 32, (uint32_t)irq_stub_table[i], 0x08, 0x8E);
    }

}

/* --- Exception handler --- */
void isr_handler(registers_t r) {
    print("CPU Exception! System halted.\n");
    while (1);
}

/* --- IRQ handler --- */
void irq_handler(registers_t r) {

    if (r.int_no >= 40) outb(0xA0, 0x20);
    outb(0x20, 0x20);

    if (r.int_no == 33) {
        keyboard_handler(r);
    }
}
