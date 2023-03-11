#ifndef STRING_H
#define STRING_H
#include <stdint.h>
#include <stddef.h>
uint32_t strlen(const char *str);
char *strcat(char *dst, const char *src);
void *memset(void *ptr, int value, size_t num);
#endif