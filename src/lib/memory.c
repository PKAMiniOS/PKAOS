#include "../../include/pkaos.h"

void scroll() {
    uint8_t *screen = (uint8_t *)VIDEO_ADDRESS;

    // Dịch toàn bộ màn hình lên 1 dòng
    for (int i = 0; i < (MAX_ROWS - 1) * MAX_COLS * 2; i++) {
        screen[i] = screen[i + MAX_COLS * 2];
    }

    // Xóa dòng cuối
    int last = (MAX_ROWS - 1) * MAX_COLS * 2;
    for (int i = last; i < MAX_ROWS * MAX_COLS * 2; i += 2) {
        screen[i] = ' ';
        screen[i + 1] = WHITE_ON_BLACK;
    }
}

void *memcpy(void *dest, const void *src, int n) {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    for (int i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}