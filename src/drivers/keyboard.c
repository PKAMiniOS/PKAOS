#include "pkaos.h"

/* Bảng tra cứu Scancode Map - ánh xạ scancode sang ký tự ASCII */
static unsigned char kbd_us[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* Hàm xử lý sự kiện bàn phím */
void keyboard_handler(registers_t *regs)
{
    /* Đọc mã quét (scancode) từ cổng 0x60 */
    uint8_t scancode = inb(0x60);

    /* Kiểm tra nếu scancode < 0x80 (phím được nhấn xuống, không phải nhả phím) */
    if (scancode < 0x80)
    {
        /* Lấy ký tự tương ứng từ bảng scancode */
        char c = kbd_us[scancode];

        if (c != 0)
        {
            /* Xử lý các phím đặc biệt */
            switch (scancode)
            {
            case 0x1C: /* Phím Enter (0x1C) */
                print("\n");
                break;
            case 0x0E: /* Phím Backspace (0x0E) */
                /* Tạo chuỗi "backspace" command */
                print("\b");
                break;
            case 0x39: /* Phím Space (0x39) */
                print(" ");
                break;
            default:
                /* In ký tự thường */
                char buf[2] = {c, '\0'};
                print(buf);
                break;
            }
        }
    }
}

/* Hàm khởi tạo bàn phím */
void init_keyboard()
{
    /* Khởi tạo bàn phím - có thể mở khóa IRQ1 hoặc cấu hình khác nếu cần */
    /* Hiện tại chỉ khai báo hàm, logic thực tế sẽ được xử lý từ trong interrupt handler */
}
