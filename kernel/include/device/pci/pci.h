#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint8_t bus;
  uint8_t slot;
  uint8_t func;
} _pci_device_t;

typedef struct {
  uint32_t id;
  char *name;
} _pci_vendor_t;

#define PCI_VENDOR(id, name) {id, name},

uint32_t pci_read(_pci_device_t pci_info, uint8_t offset);
uint16_t pci_enumerate_devices(_pci_device_t *pci_infos);
uint16_t pci_get_vendor_id(_pci_device_t pci_info);
uint8_t pci_get_class_id(_pci_device_t pci_info);
