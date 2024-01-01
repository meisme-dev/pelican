#include "kernel.h"
#include <common/cpu/cpu.h>
#include <common/device/pci/pci.h>
#include <common/device/pci/vendors.h>
#include <common/exception/panic.h>
#include <common/io/serial/serial.h>
#include <common/memory/paging.h>
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
  paging_init();
  asm volatile("int $0x80");
  cpu_halt();
}
