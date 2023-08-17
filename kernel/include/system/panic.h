#pragma once

#include <stdint.h>
#include <stddef.h>

void _panic(const char *file, size_t line, char *format, ...);
#define panic(...) _panic(__FILE__, __LINE__, __VA_ARGS__)
