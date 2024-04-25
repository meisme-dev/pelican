#include "kernel.h"
#include <common/cpu/cpu.h>
#include <common/device/pci/pci.h>
#include <common/device/pci/vendors.h>
#include <common/io/serial/serial.h>
#include <common/memory/paging.h>
#include <exception/panic.h>
#include <memory/pmm.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

void kmain(void) {
  terminal_init();
  log_init(LOGLEVEL);
  if (!(pmm_init())) {
    panic("FAILED TO INTIALIZE PHYSICAL MEMORY MANAGER");
  }
  log_print(DEBUG, "Total memory: %u", pmm_get_total_mem());
  cpu_init();
  paging_init();
  asm volatile("int $0x80");
  cpu_halt();
}
