#ifndef PKAOS_H
#define PKAOS_H

/* Kiểu dữ liệu */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

/* Cấu trúc tập tin đăng ký (registers) */
typedef struct
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

/* Hàm giao tiếp cổng I/O */
uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);

/* Hàm điều khiển màn hình */
void clear_screen();
void print(const char *str);

/* Hàm xử lý bàn phím */
void keyboard_handler(registers_t *regs);
void init_keyboard();

#endif