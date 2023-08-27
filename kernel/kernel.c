#include "kernel.h"
#include <common/boot/init.h>
#include <common/device/pci/pci.h>
#include <common/device/pci/vendors.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

void kmain(void) {
  kinit();
  asm volatile("int $0x80");
  while (1) {
    asm volatile("cli");
    asm volatile("hlt");
  }
}
