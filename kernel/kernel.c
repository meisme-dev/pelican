#include "kernel.h"
#include "memory/vmm.h"
#include <common/cpu/cpu.h>
#include <common/device/pci/pci.h>
#include <common/device/pci/vendors.h>
#include <common/exception/panic.h>
#include <common/io/serial/serial.h>
#include <memory/pmm.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

void kmain(void) {
  terminal_init();
  log_init(LOGLEVEL);
  cpu_init();
  if (!(pmm_init())) {
    panic("FAILED TO INTIALIZE PHYSICAL MEMORY MANAGER");
  }
  page_header_t *test = vmm_alloc_mem(8192);
  if (!test) {
    panic("null");
  }
  page_header_t *current = test;

  while (current->next) {
    printf("%x", current->base);
    current = current->next;
  }
  asm volatile("int $0x80");
  cpu_halt();
}
