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
  if (!(pmm_init())) {
    panic("FAILED TO INTIALIZE PHYSICAL MEMORY MANAGER");
  }
  cpu_init();
  page_descriptor_t *page = vmm_alloc_mem(16 * PAGE_SIZE);
  if (!page) {
    panic("COULD NOT ALLOCATE MEMORY");
  }
  page_descriptor_t *current = page;
  log(DEBUG, "Allocated pages:");
  while (current->next) {
    log(DEBUG, "    base: 0x%x, size: 0x%x", current->base, PAGE_SIZE);
    current = current->next;
  }
  vmm_free_mem(page);
  asm volatile("int $0x80");
  cpu_halt();
}
