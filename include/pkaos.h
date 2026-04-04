#ifndef PKAOS_H
#define PKAOS_H

/* Kiểu dữ liệu */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

/* Hàm giao tiếp cổng I/O */
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);

/* Hàm điều khiển màn hình */
void clear_screen();
void print(const char* str);
void putchar(char c);
void backspace();

/* ================= GDT ================= */
typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

/* ================= IDT ================= */
typedef struct {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_hi;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

/* ================= REGISTERS ================= */
typedef struct {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

/* ================= PROTOTYPES ================= */

// Init
void gdt_init();
void idt_init();
void interrupt_init(); 
void isr_init();
void init_keyboard();
void keyboard_handler(registers_t r);

// IDT helper
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// ASM
extern void gdt_flush(uint32_t gdt_ptr_addr);
extern void idt_load(uint32_t idt_ptr_addr);

/* stdlib */
float my_sqrt(float x);
float my_atof(const char *str);
void ftoa(float num, char *str, int precision);

/* string */
uint32_t strcmp(const uint8_t *s1, const uint8_t *s2);

/* shell */
void shell_init();
void shell_handle_char(char ch);
void shell_handle_backspace();
void shell_handle_enter();

#endif
