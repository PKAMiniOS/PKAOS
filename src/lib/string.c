#include <pkaos.h>

/* Dùng uint32_t để đảm bảo độ dài chuỗi có thể lên đến 4 tỷ ký tự */
uint32_t strlen(const uint8_t *s) {
    uint32_t len = 0;
    while (s[len] != 0) { // 0 thay cho '\0'
        len++;
    }
    return len;
}

/* So sánh hai chuỗi sử dụng hoàn toàn uint8_t */
uint32_t strcmp(const uint8_t *s1, const uint8_t *s2) {
    uint32_t i = 0;
    while (s1[i] != 0 && (s1[i] == s2[i])) {
        i++;
    }
    return (uint32_t)(s1[i] - s2[i]);
}

/* Copy chuỗi dùng uint32_t cho biến đếm i */
void strcpy(uint8_t *dest, const uint8_t *src) {
    uint32_t i = 0;
    while (src[i] != 0) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0; // Kết thúc chuỗi bằng byte 0
}
