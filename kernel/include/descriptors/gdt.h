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
  uint16_t size;
  uint64_t offset;
} _gdtr_t;

typedef struct __attribute__((__packed__)) {
  uint32_t prev_tss;
  uint32_t esp0;
  uint32_t ss0;
  uint32_t esp1;
  uint32_t ss1;
  uint32_t esp2;
  uint32_t ss2;
  uint32_t cr3;
  uint32_t eip;
  uint32_t eflags;
  uint32_t eax;
  uint32_t ecx;
  uint32_t edx;
  uint32_t ebx;
  uint32_t esp;
  uint32_t ebp;
  uint32_t esi;
  uint32_t edi;
  uint32_t es;
  uint32_t cs;
  uint32_t ss;
  uint32_t ds;
  uint32_t fs;
  uint32_t gs;
  uint32_t ldt;
  uint16_t trap;
  uint16_t iomap_base;
} _tss_t;

typedef uint8_t gdt_descriptor[8];

void gdt_init();
