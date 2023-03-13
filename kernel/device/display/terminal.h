#ifndef TERMINAL_H
#define TERMINAL_H
#include "../../../vendor/limine/limine.h"
#define putchar(x) kputchar(x)

typedef struct {
    uint8_t *ptr;
    uint32_t bg;
    uint32_t fg;
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
    uint32_t p;
} TerminalInfo;

void set_terminal_font(unsigned char *src);
void set_terminal_state(TerminalInfo buffer);
void printf(char *format, ...);
void puts(char *str);
#endif