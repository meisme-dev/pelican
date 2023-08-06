#pragma once

#include <stdint.h>

void _panic(const char *file, size_t line, const char *msg);
#define panic(x) _panic(__FILE__, __LINE__, x)
