#include <pkaos.h>

/* Hàm khởi tạo Floating Point Unit (FPU) */
void fpu_init() {
    uint32_t cr0;
    // Đọc trạng thái thanh ghi CR0
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));

    // Xóa bit 2 (EM) và thiết lập bit 1 (MP) để kích hoạt FPU
    cr0 &= ~(1 << 2); 
    cr0 |= (1 << 1);
    
    // Ghi lại kết quả vào CR0
    __asm__ volatile("mov %0, %%cr0" : : "r"(cr0));

    uint32_t cr4;
    // Đọc trạng thái thanh ghi CR4
    __asm__ volatile("mov %%cr4, %0" : "=r"(cr4));
    
    // Bật bit 9 và 10 cho phép dùng tập lệnh SSE (để tính số thực nhanh hơn)
    cr4 |= (1 << 9);
    cr4 |= (1 << 10);
    
    // Ghi lại kết quả vào CR4
    __asm__ volatile("mov %0, %%cr4" : : "r"(cr4));

    // Lệnh nạp cấu hình mặc định cho FPU
    __asm__ volatile("finit"); 
}
