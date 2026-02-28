// Hàm inb, outb dùng để truy cập vào cổng I/O
#include "pkaos.h"
// Hàm inb đọc 1 byte từ cổng I/O
uint8_t inb(uint16_t port) {
    // Biến result dùng để lưu kết quả đọc được từ cổng I/O.
    uint8_t result;
    // sử dụng asm để thực hiện lệnh, ngăn trình biên dịch bỏ qua lệnh này.
    // a là thanh ghi dùng để lưu kết quả.
    // Nd dùng để chỉ cho trình biên dịch chọn cách để đưa port vào lệnh, nếu port 8 bit (0-255) thì ghi trực tiếp vào lệnh, nếu lớn hơn 255 hoặc là biến thì đặt trong thanh ghi DX
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}
// Hàm outb ghi 1 byte vào cổng I/O
void outb(uint16_t port, uint8_t data) {
    // Biến data dùng để lưu dữ liệu cần ghi vào cổng I/O.
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}