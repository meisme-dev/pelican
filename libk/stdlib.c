#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void itoa(int64_t x, char *str) {
    uint8_t i = 0;
    int8_t mult = 1;
    if(x < 0) {
        str[0] = '-';
        mult = -1;
        i++;
    }

    while(x != 0) {
        str[i] = (char)(x * mult % 10 + 48);
        x /= 10;
        i++;
    }

    for(uint64_t j = 0, i = strlen(str) - 1; j < strlen(str)/2; j++, i--) {
        char tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
    }
}
