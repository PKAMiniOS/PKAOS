#include "pkaos.h"

void *memcpy(void *dest, const void *src, int n) {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    for (int i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}
