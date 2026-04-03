#include "pkaos.h"

void kernel_main() {
    clear_screen();
    print("=== Welcome to PKAOS ===\n\n");
    init_keyboard();
    print("Keyboard initialized. Ready for input!\n");
    while(1);
}