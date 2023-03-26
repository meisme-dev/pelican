#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include <stddef.h>

#define DESC ((x) << 0x04)
#define PRES ((x) << 0x07)
#define SAVL ((x) << 0x0C)
#define LONG ((x) << 0x0D)
#define SIZE ((x) << 0x0E)
#define GRAN ((x) << 0x0F)
#define PRIV (((x) & 0x03) << 0x05)

#define D_R     0x0
#define D_RA    0x1
#define D_RW    0x2
#define D_RWA   0x3
#define D_RED   0x4
#define D_REDA  0x5
#define D_RWED  0x6
#define D_RWEDA 0x7
#define C_X     0x8
#define C_EXA   0x9
#define C_XR    0xA
#define C_XRA   0xB
#define C_XC    0xC
#define C_XCA   0xD
#define C_XRC   0xE
#define C_XRCA  0xF

typedef struct __attribute__((__packed__)) {
    uint32_t base;
    uint32_t limit;
} Gdtr;

typedef struct __attribute__((__packed__)){
    Gdtr gdtr;
    uint16_t flag;
} GdtData;

void create_descriptors(GdtData *gdt_data, uint64_t* descriptors, size_t num);

#endif
