#pragma once

#include <stdint.h>

typedef struct {
    uint32_t magic;         
    uint32_t version;       
    uint32_t header_size;    
    uint32_t flags;         
    uint32_t count;     
    uint32_t bpg; 
    uint32_t height;        
    uint32_t width;         
} _psf_font_t;

void psf_putchar(uint16_t character, uint32_t *cx, uint32_t *cy, uint32_t fg, uint32_t bg);
void psf_init(_psf_font_t *font);