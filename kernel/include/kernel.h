#pragma once

#include <device/pci/pci.h>
#include <memory/pmm.h>

typedef struct {
  _block_t *head;
  uint32_t *pci_ids;
  uint16_t pci_count;
  _pci_device_t *pci_devices;
  uint64_t mem_list_count;
} _kernel_state_t;
