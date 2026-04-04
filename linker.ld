/* 1. Khai báo cửa vào và ép chuẩn kiến trúc - tham khảo WyoOS */
/* elf32 là chuẩn, còn i386 là 32bit, có thể hiểu OUTPUT_FORMAT là đóng gói cái vỏm 
còn OUTPUT_ARCH là cái lõi i386, như kiểu là dịch nhị phân thành dạng 32bit cho máy đọc*/
ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS {
    /* 2. Bắt đầu nạp từ mốc 1MB */
    . = 1M;

    /* Multiboot header phải ở đầu tiên (trong 32KB đầu) */
    .multiboot : {
        *(.multiboot)
    }

    /* 3. Phân lô 4KB để làm Segmentation (Phân đoạn - sử dụng mô hình Flat Memory) */
    /* .text là nơi chứa toàn bộ code, trong đó chạy bằng chuẩn multiboot, sau đó là text - tất cả các file code */
    /* rodata là read-only data, ví dụ như print("hello") thì hello là rodata */
    /* data là biến toàn cục được gán, ví dụ như int x = 1 thì 1 là data */
    /* bss là mảng rỗng, hoặc các biến toàn cục chưa gán giá trị ví dụ như int a[1000]*/

    .text BLOCK(4K) : ALIGN(4K) {
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }

    /* Xóa bỏ thông tin rác của compiler - tham khảo WyoOS*/
    /DISCARD/ : {
        *(.comment)
        *(.note.GNU-stack)
        *(.fini_array*)
    }
}