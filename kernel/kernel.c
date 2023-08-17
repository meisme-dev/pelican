#include <descriptors/gdt.h>
#include <descriptors/idt.h>
#include <device/display/framebuffer.h>
#include <device/display/log.h>
#include <device/display/terminal.h>
#include <device/pci/pci.h>
#include <device/pci/vendors.h>
#include <device/serial/serial.h>
#include <memory/pmm.h>
#include <stdlib.h>
#include <string.h>
#include <system/panic.h>

typedef struct {
  _block_t *head;
  uint64_t mem_list_count;
} _kernel_state_t;

static void kstart(_kernel_state_t state);

void kinit(void) {
  if (terminal_init()) {
    log(SUCCESS, "Initialized Terminal");
  }
  
  if (serial_init(COM1)) {
    log(SUCCESS, "Initialized COM1");
  }
  
  gdt_init();
  log(SUCCESS, "Initialized GDT and TSS");
  
  idt_init();
  log(SUCCESS, "Initialized IDT");

  uint64_t count = pmm_get_blocks();

  if(count == 0) {
    panic("Failed to initialize PMM");
  }

  _block_t blocks[count];
  pmm_init(blocks);
  log(SUCCESS, "Initialized PMM");

  _kernel_state_t state;
  state.head = blocks;
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

  uint64_t free = 0, used = 0, reserved = 0;
  log(INFO, "Memory: %u blocks", state.mem_list_count);
  for (uint64_t i = 0; i < state.mem_list_count; i++) {
    switch (state.head[i].block_type) {
      case FREE:
        free++;
        break;
      case USED:
      //  log(ERROR, "This should never happen");
        used++;
        break;
      case RESERVED:
        reserved++;
        break;
    }
  }
 // log(INFO, "Free: %u, Used: %u, Reserved: %u", free, used, reserved);
  __asm__ volatile("int $0x80");
  __asm__ volatile("hlt");
}
