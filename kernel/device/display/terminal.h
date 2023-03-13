#ifndef TERMINAL_H
#define TERMINAL_H
#include "../../../vendor/limine/limine.h"

typedef struct {
    uint8_t *ptr;
    uint32_t bg;
    uint32_t fg;
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
    uint32_t p;
} Buffer;

void set_terminal_font(char *src);
void set_terminal_state(Buffer buffer);
void kputchar(const char c);
void kputs(const char *c);
#endif