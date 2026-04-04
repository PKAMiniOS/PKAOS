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

    ; Lấy int_no từ stack - offset chính xác: 36 (32 bytes pusha + 4 bytes push eax)
    mov eax, [esp + 36]
    cmp eax, 32
    jl .isr

    call irq_handler
    jmp .done

.isr:
    call isr_handler

.done:
    pop eax        ; pop ds_saved
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Manual pop registers từ pusha (thứ tự pop ngược với pusha)
    ; Pusha push: EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX
    ; Pop: EAX, ECX, EDX, EBX, (skip ESP), EBP, ESI, EDI
    pop eax    ; pop EAX từ pusha
    pop ecx    ; pop ECX
    pop edx    ; pop EDX
    pop ebx    ; pop EBX
    add esp, 4  ; skip ESP - do not restore (it was saved before interrupt occurred)
    pop ebp    ; pop EBP
    pop esi    ; pop ESI
    pop edi    ; pop EDI

    add esp, 8  ; pop int_no, error_code (từ ISR stub)
    iret        ; pop EIP, CS, EFLAGS từ CPU stack

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

; --- ISR STUB TABLE (để C code sử dụng) ---
[GLOBAL isr_stub_table]
isr_stub_table:
    dd isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
    dd isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
    dd isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
    dd isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31

; --- IRQ STUB TABLE (để C code sử dụng) ---
[GLOBAL irq_stub_table]
irq_stub_table:
    dd irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7
    dd irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15
