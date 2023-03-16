#include "../kernel/device/display/terminal.h"
#include "log.h"

void _trace(const char *file, size_t line) {
    printf("At %s:%d:\n", file, line);
}