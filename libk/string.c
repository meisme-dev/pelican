#include <string.h>

size_t strlen(const char *str) {
  size_t len = 0;
  while (*str++ != '\0') {
    len++;
  }
  return len;
}

size_t strnlen(const char *str, size_t n) {
  size_t len = 0;
  size_t num = n;
  while ((*str++ != '\0') && num--) {
    len++;
  }
  return len;
}

char *strcat(char *dst, const char *src) {
  char *tmp = dst + strlen(dst);
  while ((*dst++ = *src++))
    ;
  return tmp;
}

char *strncat(char *dst, const char *src, size_t n) {
  char *tmp = dst + strnlen(dst, n);
  while ((*dst++ = *src++) && n--)
    ;
  return tmp;
}

char *strcpy(char *dst, const char *src) {
  char *tmp = dst;
  while ((*dst++ = *src++))
    ;
  return tmp;
}

char *strncpy(char *dst, const char *src, size_t n) {
  char *tmp = dst;
  while ((*dst++ = *src++) && n--)
    ;
  *tmp = '\0';
  return tmp;
}

int strcmp(const char *str1, const char *str2) {
  while (*str1 != '0' && *str2 != '\0' && *str1 == *str2) {
    str1++;
    str2++;
  }

  return (unsigned char)*str1 - (unsigned char)*str2;
}

int strncmp(const char *str1, const char *str2, size_t n) {
  if (n == 0) {
    return 0;
  }

  while (*str1 && *str1 == *str2 && n > 1) {
    str1++;
    str2++;
    n--;
  }

  return (unsigned char)*str1 - (unsigned char)*str2;
}

void *memset(void *ptr, int value, size_t n) {
  unsigned char *b = ptr;
  do {
    *b = value;
    b++;
  } while (n-- > 0);
  return ptr;
}

void *memmove(void *dest, const void *src, size_t n) {
  const char *s = (const char *)src;
  char *d = (char *)dest;

  if (d == s) {
    return dest;
  }

  if (d > s) {
    s += n;
    d += n;

    while (n--) {
      *--d = *--s;
    }

    return dest;
  }

  while (n--) {
    *d++ = *s++;
  }

  return dest;
}