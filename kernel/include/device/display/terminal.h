#pragma once

#include <limine/limine.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

extern struct limine_framebuffer *framebuffer;

bool terminal_init(void);
void printf(char *format, ...);
void vprintf(char *format, va_list args);
void puts(const char *str);
void kputs(const char *c);
void kputchar(const char c);
void set_bold(bool bold);
void reset_pos(void);
void set_col(uint32_t bg, uint32_t fg);
void _trace(const char *file, size_t line);
#define trace() _trace(__FILE__, __LINE__)
