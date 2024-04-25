#include "kernel.h"
#include <common/cpu/cpu.h>
#include <common/device/pci/pci.h>
#include <common/device/pci/vendors.h>
#include <common/io/serial/serial.h>
#include <common/memory/vmm.h>
#include <exception/panic.h>
#include <memory/pmm.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

void kmain(void) {
  terminal_init();
  log_init(LOGLEVEL);
  cpu_init();
  pmm_init();
  vmm_init();
  log_print(DEBUG, "Total memory: %u", pmm_get_total_mem());
  asm volatile("int $0x80");
  cpu_halt();
}
