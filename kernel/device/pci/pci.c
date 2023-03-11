#include "pci.h"

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

uint16_t pci_read(uint32_t bus, uint32_t slot, uint32_t func, uint8_t offset) {
    uint32_t location = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    outl(CONFIG_ADDRESS, location);
    return (uint16_t)((inl(CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
}

