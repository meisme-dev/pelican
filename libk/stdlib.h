#pragma once

#include <stddef.h>
#include <stdint.h>
void itoa(int64_t x, char *str, uint32_t base);
void utoa(uint64_t x, char *str, uint32_t base);