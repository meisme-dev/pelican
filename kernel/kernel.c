#include <device/display/framebuffer.h>
#include <device/display/terminal.h>
#include <device/pci/pci.h>
#include <device/serial/serial.h>
#include <memory/pmm.h>
#include <stdlib.h>
#include <string.h>

void kstart(void) {
  if (init_serial(COM1)) {
    if (!init_terminal()) {
      char *message = "Failed to initialize terminal, but found serial. \n"
                      "Displaying to Serial...";
      while (*message) {
        put_serial(COM1, *message);
        message++;
      }
    }
    puts("Initialized terminal");
    puts("Initialized serial");
  }
  init_pmm();
  puts("Initialized pmm");
  for (;;)
    ;
}
