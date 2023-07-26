#ifndef PANIC_H
#define PANIC_H

#include <device/display/terminal.h>
#include <device/display/framebuffer.h>

void _panic(const char *file, size_t line, const char *msg);
#define panic(x) _panic(__FILE__, __LINE__, x)

#endif