#include "gdt.h"

void *create_descriptors(GdtData *gdt_data, uint64_t* descriptors, size_t num) {
    for(size_t i = 0; i < num; i++) {
        descriptors[i]  = gdt_data[i].gdtr.limit & 0x000F0000;
        descriptors[i] |= (gdt_data[i].flag << 8) & 0x00F0FF00;
        descriptors[i] |= (gdt_data[i].gdtr.base >> 16) & 0x000000FF;
        descriptors[i] |= gdt_data[i].gdtr.base & 0xFF000000;
        descriptors[i] <<= 32;
        descriptors[i] |= gdt_data[i].gdtr.base << 16;
        descriptors[i] |= gdt_data[i].gdtr.limit & 0x0000FFFF;
    }
}

void load_descriptor(Gdtr gdtr) {
    asm volatile("lgdt %0" :: "m"(gdtr) : "memory");
}