#pragma once

#include <stddef.h>
#include <stdint.h>

void _panic(const char *file, size_t line, char *format, ...);
#define panic(...) _panic(__FILE__, __LINE__, __VA_ARGS__)
