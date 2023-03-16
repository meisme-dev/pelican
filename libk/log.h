#ifndef LOG_H
#define LOG_H
#include <stddef.h>
void _trace(const char *file, size_t line);
#define trace() _trace(__FILE__, __LINE__)
#endif