#include <arch/amd64/boot/gdt/gdt.h>
#include <arch/amd64/cpu/cpu.h>
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
  cpu_init();
}