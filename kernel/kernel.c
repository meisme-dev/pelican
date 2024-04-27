#include "kernel.h"
#include <arch/common/cpu/cpu.h>
#include <exception/panic.h>
#include <memory/pmm.h>
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
