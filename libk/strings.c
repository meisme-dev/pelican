#include "strings.h"

uint32_t strlen(const char *str) {
    uint32_t len = 0;
    while(*str != '\0') {
        str++;
        len++;
    }
    return len;
}

char *strcat(char *dst, const char *src) {
    char *tmp = dst;
    while (*dst) {
      dst++;
    }
    while (*dst++ = *src++);
    return tmp;
}

char *strcpy(char *dst, const char *src) {
    char *tmp = dst;
    while(*dst++ = *src++);
    return tmp;
}