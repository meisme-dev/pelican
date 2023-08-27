#include <arch/amd64/boot/gdt/gdt.h>
#include <arch/amd64/exception/idt.h>
#include <arch/amd64/io/serial/serial.h>
#include <common/exception/panic.h>
#include <kernel.h>
#include <memory/pmm.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

void kinit() {
  if (!terminal_init() && !serial_init(COM1)) {
    asm volatile("hlt");
  }

  log_init(LOGLEVEL);
  gdt_init();
  idt_init();
  uint64_t count = 0;
  _block_t *first_block = pmm_get_blocks(&count);
  if ((void *)first_block == NULL) {
    panic("Failed to initialize PMM");
  }
  pmm_init(first_block, count);

  _kernel_state_t state;
  state.head = first_block;
  state.mem_list_count = count;

  kstart(state);
}