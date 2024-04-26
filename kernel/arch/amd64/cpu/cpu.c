#include <arch/amd64/boot/gdt/gdt.h>
#include <arch/amd64/exception/idt.h>
#include <cpu/cpu.h>
#include <limine/limine.h>
#include <memory/vmm.h>
#include <stddef.h>
#include <sync/lock.h>
#include <terminal/log.h>

volatile struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
    .flags = 1,
};

inline static void halt() {
  while (1) {
    static atomic_flag lock = ATOMIC_FLAG_INIT;
    acquire(&lock);
    asm volatile("cli");
    asm volatile("hlt");
    release(&lock);
  }
}

static void core_init(struct limine_smp_info *info) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  gdt_init();
  idt_init();
  release(&lock);

  vmm_init();

  if (info->lapic_id == 0) { /* Don't halt main CPU yet */
    return;
  }
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
