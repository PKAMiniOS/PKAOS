#include "pkaos.h"
//code tham khảo từ WyoOS

//  hàm để đọc 1 byte từ cổng Input vào CPU
// ý nghĩa: hãy đọc 1 byte từ cổng %1, rồi cất kết quả vào %0
uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}
//  hàm để ghi 1 byte từ cổng Output ra khỏi CPU
void outb(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

//Note: Các hàm trên được sử dụng inline assembly để thực hiện các lệnh I/O trực tiếp với phần cứng
//      giúp CPU giao tiếp với các thiết bị ngoại vi thông qua cổng I/O.

//Note2: cấu trúc của lệnh inline assembly trong C/C++ bao gồm ba phần chính:
//       __asm__ volatile ( "Lệnh_Assembly" : Đầu_ra : Đầu_vào );
// __asm__ volatile: đánh dấu đoạn mã phía sau là assembly, volatile cho compiler biết đoạn mã này KHÔNG được phép thay đổi.

//Note3:   %1 đại diện cho tham số đầu vào (port), và %0 đại diện cho tham số đầu ra (result).
// ...