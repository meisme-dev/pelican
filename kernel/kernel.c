#include "kernel.h"
#include <common/boot/init.h>
#include <common/device/pci/pci.h>
#include <common/device/pci/vendors.h>
#include <common/exception/panic.h>
#include <memory/pmm.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

void kmain(void) {
  kinit();
  if (!(pmm_init())) {
    panic("FAILED TO INTIALIZE PHYSICAL MEMORY MANAGER");
  }
  page_header_t *header;
  if (!(header = pmm_alloc_block())) {
    panic("UNABLE TO ALLOCATE MEMORY");
  }
  log(DEBUG, "Allocated block at address 0x%x", header->base);
  pmm_free_block(header);
  if (header->block_type == FREE) {
    log(DEBUG, "Freed block at address 0x%x", header->base);
  }
  asm volatile("int $0x80");
  while (1) {
    asm volatile("cli");
    asm volatile("hlt");
  }
}
