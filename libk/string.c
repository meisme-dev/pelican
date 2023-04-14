#include <string.h>

size_t strlen(const char *str) {
    size_t len = 0;
    while(*str++ != '\0') {
        len++;
    }
    return len;
}

size_t strnlen(const char *str, size_t n) {
    size_t len = 0;
    size_t num = n;
    while((*str++ != '\0') && num--) {
        len++;
    }
    return len;
}

char *strcat(char *dst, const char *src) {
    char *tmp = dst + strlen(dst);
    while ((*dst++ = *src++));
    return tmp;
}

char *strncat(char *dst, const char *src, size_t n) {
    char *tmp = dst + strnlen(dst, n);
    while ((*dst++ = *src++) && n--);
    return tmp;
}

char *strcpy(char *dst, const char *src) {
    char *tmp = dst;
    while((*dst++ = *src++));
    return tmp;
}

char *strncpy(char *dst, const char *src, size_t n) {
    char *tmp = dst;
    while((*dst++ = *src++) && n--);
    *tmp = '\0';
    return tmp;
}

void *memset(void *ptr, int value, size_t n) {
    for(uint64_t i = 0; i < n; i++) {
        *((uint64_t *)ptr + i) = value;
    }
    return ptr;
}
