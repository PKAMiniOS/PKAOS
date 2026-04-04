#include "../../include/pkaos.h"

/* ================= SQRT ================= */
float my_sqrt(float x) {
    if (x <= 0.0f) return 0.0f;

    float guess = (x > 1.0f) ? x : 1.0f;
    float epsilon = 0.0001f;

    while (1) {
        float next = 0.5f * (guess + x / guess);
        float diff = next * next - x;
        if (diff < 0.0f) diff = -diff;

        if (diff <= epsilon) {
            return next;
        }

        guess = next;
    }
}

/* ================= ATOF ================= */
float my_atof(const char *str) {
    if (!str) return 0.0f;

    int i = 0;

    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
        i++;
    }

    int sign = 1;
    if (str[i] == '+' || str[i] == '-') {
        if (str[i] == '-') sign = -1;
        i++;
    }

    float result = 0.0f;

    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10.0f + (str[i] - '0');
        i++;
    }

    if (str[i] == '.') {
        i++;
        float decimal = 0.1f;

        while (str[i] >= '0' && str[i] <= '9') {
            result += (str[i] - '0') * decimal;
            decimal *= 0.1f;
            i++;
        }
    }

    return result * sign;
}

/* ================= FTOA ================= */
void ftoa(float num, char *str, int precision) {
    if (!str) return;

    if (precision < 0) precision = 0;
    if (precision > 6) precision = 6;

    int i = 0;

    if (num < 0.0f) {
        str[i++] = '-';
        num = -num;
    }

    float round = 0.5f;
    for (int p = 0; p < precision; p++) {
        round *= 0.1f;
    }
    num += round;

    int int_part = (int)num;
    float frac = num - int_part;

    if (int_part == 0) {
        str[i++] = '0';
    } else {
        char temp[20];
        int j = 0;

        while (int_part > 0 && j < 19) {
            temp[j++] = '0' + (int_part % 10);
            int_part /= 10;
        }

        while (j--) {
            str[i++] = temp[j];
        }
    }

    if (precision > 0) {
        str[i++] = '.';

        for (int k = 0; k < precision; k++) {
            frac *= 10.0f;

            int digit = (int)(frac + 0.000001f);
            if (digit > 9) digit = 9;

            str[i++] = '0' + digit;
            frac -= digit;
        }
    }

    str[i] = '\0';
}