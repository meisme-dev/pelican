#pragma once

#if defined(amd64)
#include <arch/amd64/device/pci/pci.h>
#else
#error "This architecture is not supported!"
#endif

typedef struct {
  uint32_t id;
  char *name;
} _pci_vendor_t;

#define PCI_VENDOR(id, name) \
  { id, name }
