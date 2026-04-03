#include "pkaos.h"

/* Hàm xử lý các yêu cầu ngắt (IRQ) */
void irq_handler(registers_t *regs)
{
    /* Kiểm tra nếu là ngắt keyboard (IRQ1 = int_no 33 = 0x21) */
    if (regs->int_no == 33)
    {
        /* Gọi hàm xử lý bàn phím */
        keyboard_handler(regs);
    }
    /* Có thể thêm các interrupt handler khác tại đây */
}
