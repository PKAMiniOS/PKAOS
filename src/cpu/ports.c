
#include "pkaos.h"

// Hàm inb dùng để đọc 1 byte từ cổng I/O (lệnh IN)
uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

// Hàm outb dùng để ghi 1 byte ra cổng I/O (lệnh OUT)
void outb(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

// Note 1:
// - port: Địa chỉ cổng I/O mà CPU sẽ đọc hoặc ghi dữ liệu
// - result: Biến dùng để lưu kết quả đọc được từ cổng I/O
// - data: Biến dùng để lưu dữ liệu cần ghi vào cổng I/O

// Note 2:
// - "inb": Dùng để đọc dữ liệu từ cổng I/O vào thanh ghi AL
// - "outb": Dùng để ghi dữ liệu từ thanh ghi AL ra cổng I/O

// Note 3:
// - "=a": Sử dụng thanh ghi AL để nhận kết quả đọc được từ cổng I/O (IN)
// - "a": Sử dụng thanh ghi AL để chứa dữ liệu cần ghi ra cổng I/O (OUT)

// Note 4:
// - "Nd": Chỉ cho trình biên dịch chọn cách để đưa port vào lệnh
//       + Nếu port là 8-bit (0–255) thì ghi trực tiếp vào lệnh
//       + Nếu port lớn hơn 255 hoặc là biến thì đặt trong thanh ghi DX

// Note 5:
// - volatile:
//       + Ngăn trình biên dịch tối ưu hóa hoặc bỏ qua các lệnh I/O
//       + Đảm bảo lệnh I/O được thực thi đúng thứ tự.