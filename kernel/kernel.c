#include "kernel.h"
#include <common/boot/init.h>
#include <common/device/pci/pci.h>
#include <common/device/pci/vendors.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

void kmain(void) {
  kinit();
}

void kstart(_kernel_state_t state) {
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
  while (1) {
    asm volatile("cli");
    asm volatile("hlt");
  }
}
