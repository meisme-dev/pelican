#pragma once

#include <stdint.h>

void interrupt(uint16_t interrupt, uint64_t cr2, uint64_t rip);

#define interrupt_handler(x) \
  extern void interrupt_##x()

#define interrupt_handler_addr(x) (uint64_t) interrupt_##x

interrupt_handler(0x0);  /* EXC_DIVISION */
interrupt_handler(0x1);  /* EXC_DEBUG */
interrupt_handler(0x2);  /* EXC_NM_INTERRUPT */
interrupt_handler(0x3);  /* EXC_BREAKPOINT */
interrupt_handler(0x4);  /* EXC_OVERFLOW */
interrupt_handler(0x5);  /* EXC_RANGE_EXCEEDED */
interrupt_handler(0x6);  /* EXC_INVALID_OPCODE */
interrupt_handler(0x7);  /* EXC_DEVICE_NOT_AVAILABLE */
interrupt_handler(0x8);  /* EXC_DOUBLE_FAULT */
interrupt_handler(0x9);  /* EXC_SEGMENT_OVERRUN */
interrupt_handler(0xA);  /* EXC_INVALID_TSS */
interrupt_handler(0xB);  /* EXC_SEG_NOT_PRESENT */
interrupt_handler(0xC);  /* EXC_SEG_FAULT_STACK */
interrupt_handler(0xD);  /* EXC_GP_FAULT */
interrupt_handler(0xE);  /* EXC_PAGE_FAULT */
interrupt_handler(0x10); /* EXC_FLOATING_POINT */
interrupt_handler(0x11); /* EXC_ALIGNMENT_CHECK */
interrupt_handler(0x12); /* EXC_MACHINE_CHECK */
interrupt_handler(0x13); /* EXC_SIMD */
interrupt_handler(0x14); /* EXC_VIRTUALIZATION */
interrupt_handler(0x15); /* EXC_CONTROL_PROTECTION */
interrupt_handler(0x1C); /* EXC_HYPERVISOR_INJECT */
interrupt_handler(0x1D); /* EXC_VMM_COMMUNICATION */
interrupt_handler(0x1E); /* EXC_SECURITY */
interrupt_handler(0x80); /* INT_SYSCALL */

#define EXC_DIVISION 0x0
#define EXC_DEBUG 0x1
#define EXC_NM_INTERRUPT 0x2
#define EXC_BREAKPOINT 0x3
#define EXC_OVERFLOW 0x4
#define EXC_RANGE_EXCEEDED 0x5
#define EXC_INVALID_OPCODE 0x6
#define EXC_DEVICE_NOT_AVAILABLE 0x7
#define EXC_DOUBLE_FAULT 0x8
#define EXC_SEGMENT_OVERRUN 0x9
#define EXC_INVALID_TSS 0xA
#define EXC_SEG_NOT_PRESENT 0xB
#define EXC_SEG_FAULT_STACK 0xC
#define EXC_GP_FAULT 0xD
#define EXC_PAGE_FAULT 0xE
#define EXC_FLOATING_POINT 0x10
#define EXC_ALIGNMENT_CHECK 0x11
#define EXC_MACHINE_CHECK 0x12
#define EXC_SIMD 0x13
#define EXC_VIRTUALIZATION 0x14
#define EXC_CONTROL_PROTECTION 0x15
#define EXC_HYPERVISOR_INJECT 0x1C
#define EXC_VMM_COMMUNICATION 0x1D
#define EXC_SECURITY 0x1E
#define INT_SYSCALL 0x80

typedef struct __attribute__((packed)) {
  uint16_t offset0;
  uint16_t ss;
  uint8_t ist : 3;
  uint8_t reserved0 : 5;
  uint8_t flags;
  uint16_t offset1;
  uint32_t offset2;
  uint32_t reserved1;
} idt_entry_t;

void idt_init();

void idt_load(uint16_t limit, uint64_t offset);

uint64_t exc_get_error();
uint64_t exc_get_cr2();
uint64_t exc_get_rip();
