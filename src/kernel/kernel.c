#include "pkaos.h"

void kernel_main() {
    clear_screen();
    print("=== Welcome to PKAOS ===\n\n");

    // Thiết lập bảo vệ bộ nhớ và ngắt
    gdt_init();
    idt_init();
    isr_init();

    // Khởi tạo bàn phím
    init_keyboard();

    // Cho phép ngắt phần cứng (keyboard, timer, v.v.)
    asm volatile("sti");

    // Vòng lặp chính: chờ ngắt, giảm tải CPU
    while (1) {
        asm volatile("hlt");
    }
}
