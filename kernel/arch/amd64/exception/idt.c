#include "idt.h"
#include <exception/panic.h>
#include <terminal/log.h>

idt_entry_t idt_entries[256] = {0};

void interrupt(uint16_t interrupt, uint64_t cr2) {
  uint32_t error = exc_get_error() & 0xffffffff;
  switch (interrupt) {
    case EXC_ALIGNMENT_CHECK:
      panic("ALIGNMENT CHECK");
      break;
    case EXC_PAGE_FAULT:
      panic("PAGE FAULT:\n    ADDRESS: 0x%x\n    ERROR: 0b%b", cr2, error & 0xff);
      break;
    case INT_SYSCALL:
      log_print(DEBUG, "Recieved a system call");
      break;
    case EXC_GP_FAULT:
      panic("GENERAL PROTECTION FAULT");
      break;
    default:
      panic("UNHANDLED INTERRUPT");
      break;
  }
  return;
}

static void idt_set(idt_entry_t *entry, uint64_t handler) {
  entry->offset0 = handler & 0xffff;
  entry->ss = 0x8;
  entry->ist = 0x0;
  entry->flags = 0x8e;
  entry->offset1 = (handler >> 16) & 0xffff;
  entry->offset2 = (handler >> 32) & 0xffffffff;
  entry->reserved0 = 0x0;
  entry->reserved1 = 0x0;
}

void idt_init(void) {
  idt_set(&idt_entries[EXC_DIVISION], interrupt_handler_addr(0x0));
  idt_set(&idt_entries[EXC_DEBUG], interrupt_handler_addr(0x1));
  idt_set(&idt_entries[EXC_NM_INTERRUPT], interrupt_handler_addr(0x2));
  idt_set(&idt_entries[EXC_BREAKPOINT], interrupt_handler_addr(0x3));
  idt_set(&idt_entries[EXC_OVERFLOW], interrupt_handler_addr(0x4));
  idt_set(&idt_entries[EXC_RANGE_EXCEEDED], interrupt_handler_addr(0x5));
  idt_set(&idt_entries[EXC_INVALID_OPCODE], interrupt_handler_addr(0x6));
  idt_set(&idt_entries[EXC_DEVICE_NOT_AVAILABLE], interrupt_handler_addr(0x7));
  idt_set(&idt_entries[EXC_DOUBLE_FAULT], interrupt_handler_addr(0x8));
  idt_set(&idt_entries[EXC_SEGMENT_OVERRUN], interrupt_handler_addr(0x9));
  idt_set(&idt_entries[EXC_INVALID_TSS], interrupt_handler_addr(0xA));
  idt_set(&idt_entries[EXC_SEG_NOT_PRESENT], interrupt_handler_addr(0xB));
  idt_set(&idt_entries[EXC_SEG_FAULT_STACK], interrupt_handler_addr(0xC));
  idt_set(&idt_entries[EXC_GP_FAULT], interrupt_handler_addr(0xD));
  idt_set(&idt_entries[EXC_PAGE_FAULT], interrupt_handler_addr(0xE));
  idt_set(&idt_entries[EXC_FLOATING_POINT], interrupt_handler_addr(0x10));
  idt_set(&idt_entries[EXC_ALIGNMENT_CHECK], interrupt_handler_addr(0x11));
  idt_set(&idt_entries[EXC_MACHINE_CHECK], interrupt_handler_addr(0x12));
  idt_set(&idt_entries[EXC_SIMD], interrupt_handler_addr(0x13));
  idt_set(&idt_entries[EXC_VIRTUALIZATION], interrupt_handler_addr(0x14));
  idt_set(&idt_entries[EXC_CONTROL_PROTECTION], interrupt_handler_addr(0x15));
  idt_set(&idt_entries[EXC_HYPERVISOR_INJECT], interrupt_handler_addr(0x1C));
  idt_set(&idt_entries[EXC_VMM_COMMUNICATION], interrupt_handler_addr(0x1D));
  idt_set(&idt_entries[EXC_SECURITY], interrupt_handler_addr(0x1E));
  idt_set(&idt_entries[INT_SYSCALL], interrupt_handler_addr(0x80));
  idt_load(sizeof(idt_entries) - 1, (uint64_t)&idt_entries);
  // log(SUCCESS, "Loaded IDT at 0x%x", (uint64_t)&idt_entries);
}
