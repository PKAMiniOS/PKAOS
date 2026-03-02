; Định nghĩa hằng số (giống define trong C)
MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
FLAGS    equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)


; Multiboot header: thông tin mà bootloader cần để nhận diện kernel
section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM


; Tạo vùng dữ liệu riêng cho kernel thực thi
section .bss
align 16
stack_bottom:
resb 16384 ; 16 KB cho Stack
stack_top:


; Vùng chứa code thực thi
section .text
global _start
extern kernel_main


; Hàm start dùng để: 
; - Thiết lập stack, treo CPU khi kết thúc 
_start:
    mov esp, stack_top
    call kernel_main
    cli
.hang:
    hlt
    jmp .hang



; =========================================================================
; GRUB (GNU GRUB – GRand Unified Bootloader) là một bootloader mã nguồn mở do dự án GNU phát triển. 
; Đây là phần mềm đầu tiên chạy khi máy tính khởi động, trước cả hệ điều hành.
;
; Note 1: 
; - MAGIC: giá trị để GRUB nhận diện kernel hợp lệ
; - FLAGS: các cờ yêu cầu bootloader
; - CHECKSUM: đảm bảo MAGIC + FLAGS + CHECKSUM = 0
;
; Note 2: 
; - stack_top, stack_bottom: nhãn đánh dấu đỉnh/ đáy stack
; - align 16: căn chỉnh theo 16byte để tối ưu CPU
; - resb 16384: dành ra 16KB cho stack
;
; Note 3: 
; - global _start: khai báo điểm entry, giúp linker nhận ra điểm bắt đầu
; - extern kernel_main: import hàm kernel_main từ file kernel.c
; - cli: tắt trạng thái ngắt
; - .hang: nhãn vòng lặp vô hạn
; - hlt: đưa CPU vào trạng thái nghỉ
; - jmp .hang: đệ quy gọi lại .hang -> giữ CPU trong vòng lặp vô hạn sau khi kernel_main kết thúc