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

//Note 1: Các hàm trên được sử dụng inline assembly để thực hiện các lệnh I/O trực tiếp với phần cứng
//      giúp CPU giao tiếp với các thiết bị ngoại vi thông qua cổng I/O.

//Note 2: cấu trúc của lệnh inline assembly trong C/C++ bao gồm ba phần chính:
//       __asm__ volatile ( "Lệnh_Assembly" : Đầu_ra : Đầu_vào );
// __asm__ volatile: đánh dấu đoạn mã phía sau là assembly, volatile cho compiler biết đoạn mã này KHÔNG được phép thay đổi.

//Note 3: %1 đại diện cho tham số đầu vào (port), và %0 đại diện cho tham số đầu ra (result).
//      - "inb %1, %0" có nghĩa là đọc một byte từ cổng được chỉ định bởi %1 (port) và lưu kết quả vào %0 (result).
//      - %1 đại diện cho tham số đầu vào (port), và %0 đại diện cho tham số đầu ra (result).
//      -> Sau khi đọc xong, dữ liệu được đưa vào thanh ghi A, compiler sẽ gán giá trị trong A cho biến %0 (result)
//      - "outb %0, %1" có nghĩa là ghi một byte từ thanh ghi A vào cổng được chỉ định bởi %1 (port).
//      - %0 đại diện cho tham số đầu vào (data), và %1 đại diện cho tham số đầu vào (port).
//      -> Sau khi lệnh outb chạy, dữ liệu trong thanh ghi A sẽ được gửi đến cổng I/O được chỉ định bởi %1 (port).

//Note 4: "=a" (result)
//      - "a" yêu cầu compiler sử dụng thanh ghi A để lưu trữ giá trị của biến result, tùy thuộc vào kích thước result.
//      - Dấu "=" cho biết đây là một tham số đầu ra, có nghĩa là giá trị của result sẽ được gán sau khi lệnh assembly thực thi.
//      -> Sau khi lệnh inb chạy xong, dữ liệu được đọc từ cổng sẽ được lưu vào thanh ghi A, compiler sẽ gán giá trị trong thanh ghi A cho biến result.

//Note 5: "a" (data)
//      - "a" yêu cầu compiler sử dụng thanh ghi A để lưu trữ giá trị của biến data, tùy thuộc vào kích thước data.
//      - Không có dấu "=" vì đây là một tham số đầu vào, có nghĩa là giá trị của data sẽ được sử dụng trong lệnh assembly nhưng không được gán sau khi lệnh thực thi.
//      -> Khi lệnh outb chạy, dữ liệu trong biến data sẽ được đưa vào thanh ghi A, và sau đó được gửi đến cổng được chỉ định bởi %1 (port).

//Note 6: "Nd" (port)
//      - "N" cho phép compiler sử dụng một hằng số 8-bit nếu port là một hằng số nhỏ (0-255), giúp tối ưu hóa lệnh assembly.
//      - "d" cho phép compiler sử dụng thanh ghi DX nếu port là một hằng số lớn hơn 255 hoặc một biến.
//      -> Khi lệnh inb hoặc outb chạy, compiler sẽ quyết định sử dụng hằng số 8-bit hoặc thanh ghi DX để lưu trữ giá trị của port, tùy thuộc vào giá trị của port.
