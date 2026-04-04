; --- KHAI BÁO NGOẠI GIAO ---
[EXTERN isr_handler]
[EXTERN irq_handler]

; --- GDT & IDT LOADING ---
[GLOBAL gdt_flush]
gdt_flush:
    mov eax, [esp + 4] 
    lgdt [eax]         
    mov ax, 0x10       
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush_done
.flush_done:
    ret

[GLOBAL idt_load]
idt_load:
    mov eax, [esp + 4] 
    lidt [eax]         
    ret

; --- COMMON STUB DUY NHẤT ---
common_stub:
    pusha
    mov ax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; int_no
    mov eax, [esp + 36]
    cmp eax, 32
    jl .isr

    call irq_handler
    jmp .done

.isr:
    call isr_handler

.done:
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8
    iret

; --- MACRO ---
%macro ISR_NOERR 1
[GLOBAL isr%1]
isr%1:
    push byte 0
    push byte %1
    jmp common_stub
%endmacro

%macro ISR_ERR 1
[GLOBAL isr%1]
isr%1:
    push byte %1
    jmp common_stub
%endmacro

%macro IRQ_MAC 2
[GLOBAL irq%1]
irq%1:
    push byte 0
    push byte %2
    jmp common_stub
%endmacro

; --- AUTO GENERATE ISR ---
%assign i 0
%rep 32
    %if i == 8 || i == 10 || i == 11 || i == 12 || i == 13 || i == 14 || i == 17 || i == 30
        ISR_ERR i
    %else
        ISR_NOERR i
    %endif
%assign i i+1
%endrep

; --- AUTO GENERATE IRQ ---
%assign i 0
%rep 16
    IRQ_MAC i, (i+32)
%assign i i+1
%endrep

; --- EXPORT TABLE CHO C (QUAN TRỌNG) ---
[GLOBAL isr_stub_table]
isr_stub_table:
%assign i 0
%rep 32
    dd isr%+i
%assign i i+1
%endrep

[GLOBAL irq_stub_table]
irq_stub_table:
%assign i 0
%rep 16
    dd irq%+i
%assign i i+1
%endrep
