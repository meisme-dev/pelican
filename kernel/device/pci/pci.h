#ifndef PCI_H
#define PCI_H
#include <stdint.h>
uint16_t pci_read(uint32_t bus, uint32_t slot, uint32_t func, uint8_t offset);
#endif