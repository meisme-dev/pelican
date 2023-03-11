#include "string.h"

uint32_t strlen(const char *str) {
    uint32_t len = 0;
    while(*str++ != '\0') {
        len++;
    }
    return len;
}

uint32_t strnlen(const char *str, size_t n) {
    uint32_t len = 0;
    size_t num = n;
    while((*str++ != '\0') && num--) {
        len++;
    }
    return len;
}

char *strcat(char *dst, const char *src) {
    char *tmp = dst + strlen(dst);
    while (*tmp++ = *src++);
    *tmp = '\0';
    return dst;
}

char *strncat(char *dst, const char *src, size_t n) {
    char *tmp = dst + strnlen(dst, n);
    size_t num = n - 1;
    while ((*tmp++ = *src++) && num--);
    *tmp = '\0';
    return dst;
}

char *strcpy(char *dst, const char *src) {
    char *tmp = dst;
    while(*tmp++ = *src++);
    *tmp = '\0';
    return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
    char *tmp = dst;
    size_t num = n;
    while((*tmp++ = *src++) && num--);
    *tmp = '\0';
    return dst;
}

void *memset(void *ptr, int value, size_t n) {
    for(uint32_t i = 0; i < n; i++) {
        *((uint32_t *)ptr + i) = value;
    }
    return ptr;
}