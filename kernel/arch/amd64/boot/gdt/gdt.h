#pragma once

#include <stddef.h>
#include <stdint.h>

#define D_R 0x0
#define D_RA 0x1
#define D_RW 0x2
#define D_RWA 0x3
#define D_RED 0x4
#define D_REDA 0x5
#define D_RWED 0x6
#define D_RWEDA 0x7
#define C_X 0x8
#define C_EXA 0x9
#define C_XR 0xA
#define C_XRA 0xB
#define C_XC 0xC
#define C_XCA 0xD
#define C_XRC 0xE
#define C_XRCA 0xF

typedef struct __attribute__((__packed__)) {
  uint16_t base0;
  uint16_t limit0;
  uint8_t base1;
  uint8_t access;
  uint8_t limit1 : 4;
  uint8_t flags : 4;
  uint8_t base2;
} _gdt_entry_t;

typedef struct __attribute__((__packed__)) {
  uint32_t reserved0;
  uint64_t rsp0;
  uint64_t rsp1;
  uint64_t rsp2;
  uint64_t reserved1;
  uint64_t ist1;
  uint64_t ist2;
  uint64_t ist3;
  uint64_t ist4;
  uint64_t ist5;
  uint64_t ist6;
  uint64_t ist7;
  uint64_t reserved2;
  uint16_t reserved3;
  uint16_t iopb;
} _tss_t;

typedef uint8_t gdt_descriptor[8];

void gdt_init();
void gdt_load(uint16_t limit, uint64_t offset, uint16_t tss_offset);
void gdt_segments_reload();