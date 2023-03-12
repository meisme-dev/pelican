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
uint16_t enumerate_devices(PciInfo *pci_infos);
uint16_t get_vendor_id(PciInfo pci_info);
const char *get_vendor(uint16_t vendor);

#endif