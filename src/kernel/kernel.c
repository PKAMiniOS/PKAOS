#include "pkaos.h"

void kernel_main() {
    clear_screen();
    print("=== Welcome to PKAOS ===\n\n");
    // Treo máy ở đây vì chưa có ngắt bàn phím
    
    gdt_init();
    idt_init(); 
    isr_init();

    init_keyboard();
    asm volatile("sti");
    
    while(1){
        asm volatile("hlt");
    }
}

