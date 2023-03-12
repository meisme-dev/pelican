#include "stdlib.h"
#include <stdbool.h>

void itoa(size_t x, char *str) {
    uint8_t i = 0;
    size_t r = 0;
    int8_t mult = 1;
    if(x < 0) {
        str[0] = '-';
        mult = -1;
        i++;
    }
    while (x != 0) {
        r *= 10;
        r += (x % 10);
        x /= 10;
    }

    while(r != 0) {
        str[i] = (char)(r * mult % 10 + 48);
        r /= 10;
        i++;
    }
}