#ifndef STRING_H
#define STRING_H
#include <stdint.h>
#include <stddef.h>
size_t strlen(const char *str);
size_t strnlen(const char *str, size_t n);
char *strcat(char *dst, const char *src);
char *strncat(char *dst, const char *src, size_t n);
char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, size_t n);
void *memset(void *ptr, int value, size_t n);
#endif
