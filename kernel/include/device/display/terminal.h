#ifndef TERMINAL_H
#define TERMINAL_H
#include <limine/limine.h>
#include <stddef.h>
#include <stdbool.h>

extern struct limine_framebuffer *framebuffer;

bool init_terminal(void);
void printf(char *format, ...);
void puts(char *str);
void kputchar(const char c);
void reset_pos(void);
void set_col(uint32_t bg, uint32_t fg);
void _trace(const char *file, size_t line);
#define trace() _trace(__FILE__, __LINE__)
#endif
