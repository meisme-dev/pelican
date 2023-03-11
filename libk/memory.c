#include "memory.h"
#include <stdint.h>

void *memset(void *ptr, int value, size_t num) {
    for(uint32_t i = 0; i < num; i++) {
        *((uint32_t *)ptr + i) = value;
    }
    return ptr;
}