#include "kernel.h"
#include <cpu/cpu.h>
#include <device/pci/pci.h>
#include <device/pci/vendors.h>
#include <exception/panic.h>
#include <io/serial/serial.h>
#include <memory/pmm.h>
#include <memory/vmm.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

void kmain(void) {
  terminal_init();
  log_init(LOGLEVEL);
  pmm_init();
  cpu_init();
  log_print(DEBUG, "Total memory: %u", pmm_get_total_mem());
  asm volatile("int $0x80");
  cpu_halt();
}
