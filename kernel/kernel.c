#include <descriptors/gdt.h>
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
  uint32_t *pci_ids;
  uint16_t pci_count;
  _pci_device_t *pci_devices;
  uint64_t mem_list_count;
} _kernel_state_t;


static void kstart(_kernel_state_t state);

void kinit(void) {
  if (terminal_init()) {
    log(SUCCESS, "Initialized Terminal");
  }
  if (serial_init(COM1)) {
    log(SUCCESS, "Initialized COM1");
    char *message = "Test";
    while (*message) {
      serial_send(COM1, *message);
      message++;
    }
  }
  gdt_init();
  log(SUCCESS, "Initialized GDT");
  _block_t *head = (_block_t *)NULL;
  uint64_t count = 0;
  if (!(head = (_block_t *)pmm_init(&count))) {
    panic("Failed to initialize PMM");
  }
  log(SUCCESS, "Initialized PMM");
  _pci_device_t pci_devices[32];
  uint32_t pci_ids[32];
  uint16_t pci_count = pci_enumerate_devices(pci_devices);
  for (uint16_t i = 0; i < pci_count; i++) {
    pci_ids[i] = pci_get_vendor_id(pci_devices[i]);
  }
  log(SUCCESS, "Initialized PCI");
  _kernel_state_t state;
  state.head = head;
  state.pci_count = pci_count;
  state.pci_ids = pci_ids;
  state.pci_devices = pci_devices;
  state.mem_list_count = count;
  kstart(state);
}

static void kstart(_kernel_state_t state) {
  log(INFO, "Resolution: %ux%ux%u", framebuffer->width, framebuffer->height, framebuffer->bpp);
  log(INFO, "Memory: %u bytes", state.mem_list_count * BLOCK_SIZE);
  for (uint16_t i = 0; i < state.pci_count; i++) {
    for (uint32_t j = 0; j < 2338; j++) {
      if (state.pci_ids[i] == vendors[j].id) {
        log(INFO, "PCI: %s 0%u:0%u.%u", vendors[j].name, state.pci_devices[i].bus, state.pci_devices[i].slot, state.pci_devices[i].func);
      }
    }
  }
  __asm__ volatile("hlt");
}
