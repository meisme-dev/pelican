#include <device/display/framebuffer.h>
#include <device/display/terminal.h>
#include <device/pci/pci.h>
#include <device/serial/serial.h>
#include <descriptors/gdt.h>
#include <memory/pmm.h>
#include <stdlib.h>
#include <string.h>

void kstart(void);

void kinit(void) {
  if (init_serial(COM1)) {
    if (!init_terminal()) {
      char *message = "Failed to initialize terminal, but found serial. \n"
                      "Displaying to Serial...";
      while (*message) {
        serial_send(COM1, *message);
        message++;
      }
    }
    puts("Initialized terminal");
    puts("Initialized serial");
    init_gdt();
    puts("Initialized GDT");
    uint64_t count;
    init_pmm(&count);
    puts("Initialized PMM");
    kstart();
  }
}

  void kstart(void) {
    __asm__ volatile("hlt");
  }
