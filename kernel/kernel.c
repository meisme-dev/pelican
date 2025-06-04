#include "kernel.h"
#include <arch/common/cpu/cpu.h>
#include <exception/panic.h>
#include <memory/pmm.h>
#include <task/task.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

static task_t kernel_task;

void kmain(void) {
  terminal_init();
  log_init(LOGLEVEL);
  pmm_init();
  cpu_init();
  log_print(SUCCESS, "Detected memory: %u", pmm_get_total_mem());
  asm volatile("int $0x80");
  cpu_halt();
}
