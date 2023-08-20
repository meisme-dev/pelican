#pragma once
#include <limine/limine.h>
struct limine_framebuffer *framebuffer_create(void);
void put_pixel(uint32_t x, uint32_t y, uint32_t c, struct limine_framebuffer *fb);