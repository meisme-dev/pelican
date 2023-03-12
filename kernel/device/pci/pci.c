#include "pci.h"
#include "../display/terminal.h"
#include "vendors.h"
#include <stddef.h>

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

uint32_t pci_read(PciInfo pci_info, uint8_t offset) {
    uint32_t bus = (uint32_t)pci_info.bus;
    uint32_t slot = (uint32_t)pci_info.slot;
    uint32_t func = (uint32_t)pci_info.func;

    uint32_t location = (0x80000000) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
    outl(CONFIG_ADDRESS, location);
    return inl(CONFIG_DATA);
}

uint16_t get_vendor_id(PciInfo pci_info) {
    return (uint16_t)pci_read(pci_info, 0);
}

bool device_exists(PciInfo pci_info) {
    uint32_t vendor = get_vendor_id(pci_info);
    if(vendor == 0xFFFFFFFF) return false;
    if(vendor == NULL) return false;
    return true;
}

const char *get_vendor(uint16_t vendor) {
    switch(vendor) {
        case INTEL:
        return "Intel Corporation";
        break;
        case AMD:
        return "Advanced Micro Devices, Inc.";
        break;
        case NVIDIA:
        return "NVIDIA Corporation";
        break;
        case VIRTIO:
        return "VirtIO";
        break;
        case REALTEK:
        return "Realtek Semiconductor Co., Ltd.";
        break;
    }
}

bool enumerate_devices(PciInfo *pci_infos) {
    uint16_t bus;
    uint8_t device;
    uint16_t i = 0;
    PciInfo pci_info;
    for (bus = 0; bus < 32; bus++) {
        for (device = 0; device < 4; device++) {
            pci_info.bus = bus;
            pci_info.slot = device;
            if(device_exists(pci_info) == true) {
                pci_infos[i] = pci_info;
                i++;
            }
        }
    }
    if(i == 0) {
        return false;
    }
    return true;
}