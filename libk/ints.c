#include "ints.h"
#include <stdbool.h>

uint32_t intcat_32(uint32_t first, uint32_t second) {
    uint32_t pow = 10;
    while(second >= pow) {
        pow *= 10;
    }
    return first * pow + second;
}

uint8_t intlen_32(uint32_t x) {
    if(x >= 10000) return 5;
    if(x >= 1000) return 4;
    if(x >= 100) return 3;
    if(x >= 10) return 2;
    return 1;
}

void itoa_32(int32_t x, char result[255]) {
    uint8_t i = 0;
    int32_t r = 0;
    int8_t mult = 1;
    if(x < 0) {
        result[0] = '-';
        mult = -1;
        i++;
    }
    while (x != 0) {
        r *= 10;
        r += (x % 10);
        x /= 10;
    }

    while(r != 0) {
        result[i] = (char)(r * mult % 10 + 48);
        r /= 10;
        i++;
    }
}