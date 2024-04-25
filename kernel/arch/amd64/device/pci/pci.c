#include "pci.h"
#include <arch/amd64/io/io.h>

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

uint32_t pci_read(pci_device_t pci_info, uint8_t offset) {
  uint32_t bus = (uint32_t)pci_info.bus;
  uint32_t slot = (uint32_t)pci_info.slot;
  uint32_t func = (uint32_t)pci_info.func;
  uint32_t location = (0x80000000) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
  outl(CONFIG_ADDRESS, location);
  return inl(CONFIG_DATA);
}

uint16_t pci_get_vendor_id(pci_device_t pci_info) {
  return (uint16_t)pci_read(pci_info, 0);
}

uint8_t pci_get_class_id(pci_device_t pci_info) {
  return (uint8_t)(pci_read(pci_info, 8)) >> 8;
}

bool device_exists(pci_device_t pci_info) {
  uint16_t vendor = pci_get_vendor_id(pci_info);
  if (vendor == 0xffff) return false;
  return true;
}

/* TODO: Fix buffer overflow if not enough memory allocated */
uint16_t pci_enumerate_devices(pci_device_t *pci_infos) {
  pci_device_t pci_info;
  uint16_t i = 0;
  for (uint16_t bus = 0; bus < 256; bus++) {
    for (uint8_t device = 0; device < 32; device++) {
      for (uint8_t func = 0; func < 8; func++) {
        pci_info.bus = bus;
        pci_info.slot = device;
        pci_info.func = func;
        if (device_exists(pci_info) == true) {
          pci_infos[i] = pci_info;
          i++;
        }
      }
    }
  }
  return i;
}
