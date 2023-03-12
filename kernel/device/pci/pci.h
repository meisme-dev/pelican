#ifndef PCI_H
#define PCI_H
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t bus;
    uint8_t slot; 
    uint8_t func; 
} PciInfo;


uint32_t pci_read(PciInfo pci_info, uint8_t offset);
bool enumerate_devices(PciInfo *pci_infos);
uint16_t get_vendor(PciInfo pci_info);

#endif