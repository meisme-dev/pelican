#include "kernel.h"
#include "memory/vmm.h"
#include <arch/common/cpu/cpu.h>
#include <exception/panic.h>
#include <memory/pmm.h>
#include <task/task.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

task_t *kernel_task = NULL;

void kmain(void) {
  cpu_enable_features();
  terminal_init();
  log_init(LOG_LEVEL);
  pmm_init();
  task_init();
  cpu_init();
  cpu_halt();
}
