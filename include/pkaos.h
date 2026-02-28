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

#endif