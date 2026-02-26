#include "pkaos.h"

void clear_screen() {
    uint8_t *screen = (uint8_t *)VIDEO_ADDRESS;
    for (int i = 0; i < MAX_COLS * MAX_ROWS * 2; i += 2) {
        screen[i] = ' ';
        screen[i+1] = WHITE_ON_BLACK;
    }
    cursor_offset = 0;
}

void print(const char* str) {
    uint8_t *screen = (uint8_t *)VIDEO_ADDRESS;
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] == '\n') {
            int current_row = (cursor_offset / 2) / MAX_COLS;
            cursor_offset = (current_row + 1) * MAX_COLS * 2;
        } else {
            screen[cursor_offset] = str[i];
            screen[cursor_offset+1] = WHITE_ON_BLACK;
            cursor_offset += 2;
        }
        i++;
    }
}