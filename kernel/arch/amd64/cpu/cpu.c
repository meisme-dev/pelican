// #include "arch/amd64/cpu/acpi/acpi.h"
#include "arch/amd64/cpu/cpu.h"
#include "kernel.h"
#include <arch/amd64/boot/gdt/gdt.h>
#include <arch/amd64/exception/idt.h>
#include <arch/amd64/memory/vmm.h>
#include <arch/common/cpu/cpu.h>
#include <limine/limine.h>
#include <stddef.h>
#include <stdint.h>
#include <sync/lock.h>
#include <terminal/log.h>

volatile struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
    .flags = 1,
};

inline static void halt() {
  while (1) {
    __asm__("hlt");
  }
}

void cpu_enable_features() {
  cpu_enable_sse();
  // cpu_enable_avx();
}

static void core_init(struct limine_smp_info *info) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  gdt_init();
  idt_init();
  vmm_load(((uintptr_t)kernel_task->root_page_table) - vmm_get_direct_map_base());

  release(&lock);

  if (info->lapic_id == 0) { /* Don't halt main CPU yet */
    return;
  }

  cpu_enable_features();

  log_print(INFO, "Core %u done", info->lapic_id);

  halt(); /* TODO: Replace this with scheduler call when scheduler is implemented */
}

struct limine_smp_response *cpu_init() {
  for (size_t i = 0; i < smp_request.response->cpu_count; i++) {
    /* Main CPU does not support goto_address */
    if (smp_request.response->cpus[i]->lapic_id == smp_request.response->bsp_lapic_id) {
      core_init(smp_request.response->cpus[i]);
      continue;
    }
    /* Load GDT and IDT */
    smp_request.response->cpus[i]->goto_address = core_init;
  }

  log_print(INFO, "Initializing %u cores", smp_request.response->cpu_count);

  return smp_request.response;
}

void cpu_halt() {
  for (size_t i = 0; i < smp_request.response->cpu_count; i++) {
    if (smp_request.response->cpus[i]->lapic_id == smp_request.response->bsp_lapic_id) {
      continue;
    }
    smp_request.response->cpus[i]->goto_address = halt;
  }
  halt();
}
