#include <pkaos.h>

gdt_entry_t gdt_entries[3];
gdt_ptr_t   gdt_ptr;

//hàm hỗ trợ nhập tham số gdt
void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}
//Hàm khởi tạo gdt
void gdt_init() {
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 3) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    // 1. Null segment: Bắt buộc
    gdt_set_gate(0, 0, 0, 0, 0);                
    
    // 2. Đoạn code nhân (Ring 0)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); 
    
    // 3. Đoạn dữ liệu nhân (Ring 0)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); 

    // Nạp GDT vào CPU
    gdt_flush((uint32_t)&gdt_ptr);
}

/* hàm gdt_set_gate là để tạo và điền một luật vào bảng gdt tại vị trí (num)
    Nó sẽ nhận vào các thông số như:
        + base (địa chỉ bắt đầu của vùng (logically))
        + limit (kích thước tối đa CPU được phép truy cập của vùng)
    Base sẽ được tách thành nhiều vùng nhỏ (base low, middle, high) và limit cũng được tách (limit low + 4 bit cao) 
    Gdt cũng sẽ gán "access" để phân quyền truy cập (ring 0 cho mức nhân, ring 3 cho mức người dùng)*/
