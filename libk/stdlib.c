#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void itoa(int64_t x, char *str, uint32_t base) {
  size_t i = 0;
  int8_t mult = 1;
  str[0] = '0';
  if (x < 0) {
    str[0] = '-';
    mult = -1;
    i++;
  }

  while (x != 0) {
    str[i] = x % base < 10 ? (char)((x * mult) % base + 48) : (char)((x * mult) % base + 55);
    x /= base;
    i++;
  }

  for (size_t j = 0, i = strlen(str) - 1; j < strlen(str) / 2; j++, i--) {
    char tmp = str[i];
    str[i] = str[j];
    str[j] = tmp;
  }
}

void utoa(size_t x, char *str, uint32_t base) {
  size_t i = 0;
  str[0] = '0';

  while (x != 0) {
    str[i] = x % base < 10 ? (char)(x % base + 48) : (char)(x % base + 55);
    x /= base;
    i++;
  }

  for (size_t j = 0, i = strlen(str) - 1; j < strlen(str) / 2; j++, i--) {
    char tmp = str[i];
    str[i] = str[j];
    str[j] = tmp;
  }
}
