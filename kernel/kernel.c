#include "../libk/string.h"
#include "../libk/stdlib.h"
#include "device/display/framebuffer.h"
#include "device/display/terminal.h"
#include "device/pci/pci.h"
#include "device/pci/vendors.h"


extern unsigned char _binary_assets_font_sfn_start;

void kstart() {
    struct limine_framebuffer *framebuffer = create_fb();
    memset((uint32_t *)framebuffer->address, 0x3b4252, framebuffer->width * framebuffer->height);
    PciInfo pci_info;
    char buf[16] = {'\0'};
    pci_info.bus = 0;
    pci_info.slot = 1;
    pci_info.func = 0;
    itoa(get_vendor_id(pci_info), buf);
    // if(enumerate_devices(pci_infos) == true) {
    //     strcat(buf, get_vendor(pci_infos[0]));
    // } else {
    //     strcat(buf, "No PCI devices detected");
    // }
    Buffer dst;
    dst.ptr = (uint8_t *)framebuffer->address;
    dst.bg = 0x3b4252;
    dst.fg = 0xa3be8c;
    dst.x = 16;
    dst.y = 16;
    dst.w = framebuffer->width;
    dst.h = framebuffer->height;
    dst.p = framebuffer->pitch;
    kputs(&_binary_assets_font_sfn_start, dst, buf);
    for(;;);
} 