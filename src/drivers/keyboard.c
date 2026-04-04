#include "pkaos.h"

// Bảng chuyển đổi scancode set 1 (bàn phím US) sang ký tự ASCII (chỉ xử lý phím nhấn)
static const unsigned char kbd_us[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x',
    'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void init_keyboard() {
    // Chưa cần khởi tạo gì thêm trong trình điều khiển đơn giản này
}

void keyboard_handler(registers_t r) {
    (void)r;
    
    // Đọc tất cả scancode trong buffer
    while (inb(0x64) & 1) {
        unsigned char scancode = inb(0x60);
        
        // Chỉ xử lý phím nhấn (mã nhả có bit 7 = 1)
        if (scancode & 0x80)
            continue;

        if (scancode == 0x1C) {
            // Enter
            shell_handle_enter();
        } else if (scancode == 0x0E) {
            // Backspace
            shell_handle_backspace();
        } else if (scancode == 0x39) {
            // Space
            shell_handle_char(' ');
        } else if (scancode < 128) {
            unsigned char ch = kbd_us[scancode];
            if (ch)
                shell_handle_char((char)ch); // Gửi ký tự vào shell
        }
    }
}
