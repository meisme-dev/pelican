#include <limine/limine.h>
#include <terminal/log.h>
#include <sync/lock.h>
#include <arch/amd64/exception/idt.h>
#include <arch/amd64/boot/gdt/gdt.h>

static volatile struct limine_smp_request request = {
    .id = LIMINE_SMP_REQUEST,
    .flags = 1,
    .revision = 0};

static void core_init(struct limine_smp_info *info) {
  lock();
  gdt_init();
  idt_init();
  log(SUCCESS, "CPU %u initialized", info->processor_id);
  unlock();
  while(1) {
    asm volatile("cli");
    asm volatile("hlt");
  }
}

struct limine_smp_response *cpu_init() {
  for(uint64_t i = 0; i < 16; i++) {
    if(request.response->cpus[i]->lapic_id == request.response->bsp_lapic_id) {
      continue;
    }
    request.response->cpus[i]->goto_address = core_init;
  }
  return request.response;
}

