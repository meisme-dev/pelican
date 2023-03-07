#ifndef VGA_H
#define VGA_H
#include <stdint.h>

typedef struct {
    uint16_t bg_color;
    uint16_t fg_color;
    const char *data;
    uint16_t x;
    uint16_t y;
} PrintInfo;

void kprint(PrintInfo print_info);
#endif