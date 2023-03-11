#include "string.h"

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

void *memset(void *ptr, int value, size_t num) {
    for(uint32_t i = 0; i < num; i++) {
        *((uint32_t *)ptr + i) = value;
    }
    return ptr;
}