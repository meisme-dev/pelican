#include <common/boot/gdt/gdt.h>
#include <common/device/pci/pci.h>
#include <common/device/pci/vendors.h>
#include <common/exception/idt.h>
#include <common/exception/panic.h>
#include <common/io/serial/serial.h>
#include <memory/pmm.h>
#include <stdlib.h>
#include <string.h>
#include <terminal/log.h>
#include <terminal/terminal.h>
#include <video/framebuffer.h>

#include "kernel.h"

static void kstart(_kernel_state_t state);

void kinit(void) {
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

static void kstart(_kernel_state_t state) {
  _pci_device_t pci_devices[32];
  uint32_t pci_ids[32];
  uint16_t pci_count = pci_enumerate_devices(pci_devices);
  for (uint16_t i = 0; i < pci_count; i++) {
    pci_ids[i] = pci_get_vendor_id(pci_devices[i]);
  }
  log(INFO, "Resolution: %ux%ux%u", framebuffer->width, framebuffer->height, framebuffer->bpp);
  for (uint16_t i = 0; i < pci_count; i++) {
    for (uint32_t j = 0; j < sizeof(vendors) / 16; j++) {
      if (pci_ids[i] == vendors[j].id) {
        log(INFO, "PCI: %s 0%u:0%u.%u", vendors[j].name, pci_devices[i].bus, pci_devices[i].slot, pci_devices[i].func);
      }
    }
  }

  uint64_t free = 0, used = 0;
  log(INFO, "Memory: %u bytes", state.mem_list_count * BLOCK_SIZE);
  for (uint64_t i = 0; i < state.mem_list_count; i++) {
    switch (state.head[i].block_type) {
      case FREE:
        free++;
        break;
      case USED:
        used++;
        break;
    }
  }
  log(INFO, "Free: %u, Used: %u", free * BLOCK_SIZE, used * BLOCK_SIZE);
  asm volatile("int $0x80");
  asm volatile("hlt");
}
