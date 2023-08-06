#include <descriptors/gdt.h>
#include <device/display/framebuffer.h>
#include <device/display/terminal.h>
#include <device/pci/pci.h>
#include <device/serial/serial.h>
#include <memory/pmm.h>
#include <stdlib.h>
#include <string.h>
#include <system/panic.h>

struct kernel_state {
  Block *head;
  uint64_t mem_list_count;
};

void kstart(struct kernel_state state);

void kinit(void) {
  if (init_terminal()) {
    set_col(0x0, 0x44bb66);
    printf("[INIT] ");
    set_col(0x0, 0xffffff);
    puts("Initialized Terminal");
  }
  if (init_serial(COM1)) {
    set_col(0x0, 0x44bb66);
    printf("[INIT] ");
    set_col(0x0, 0xffffff);
    puts("Initialized COM1");
    char *message = "Test";
    while (*message) {
      serial_send(COM1, *message);
      message++;
    }
  }
  gdt_init();
  uint64_t count = 0;
  if (!(head = (Block *)init_pmm(&count))) {
    panic("FAILED TO INITIALIZE PMM");
  }
  set_col(0x0, 0x44bb66);
  printf("[INIT] ");
  set_col(0x0, 0xffffff);
  puts("Initialized PMM");
  struct kernel_state state;
  state.head = head;
  state.mem_list_count = count;
  kstart(state);
}

void kstart(struct kernel_state state) {
  set_col(0x0, 0x4499FF);
  printf("[INFO] ");
  set_col(0x0, 0xffffff);
  printf("Resolution: %ux%ux%u\n", framebuffer->width, framebuffer->height, framebuffer->bpp);
  set_col(0x0, 0x4499FF);
  printf("[INFO] ");
  set_col(0x0, 0xffffff);
  printf("Memory: %u bytes\n", state.mem_list_count * BLOCK_SIZE);
  __asm__ volatile("hlt");
}
