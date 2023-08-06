#include <descriptors/gdt.h>
#include <device/display/terminal.h>

static void gdt_load_descriptor(_gdtr_t gdtr) {
  __asm__ volatile("lgdt %0" ::"m"(gdtr)
                   : "memory");
}

void gdt_init() {
  asm("cli");

  const uint8_t desc_count = 8;
  _tss_t tss = {0};
  tss.ss0 = 0x10;
  tss.iomap_base = sizeof(_tss_t);

  uint32_t tss_lower = (uint64_t)&tss & 0xffffffff;
  uint32_t tss_upper = (uint64_t)&tss >> 32;
  _gdt_entry_t gdt[desc_count];

  /* Null entry */
  gdt[0].base0 = 0x0;
  gdt[0].limit0 = 0x0;
  gdt[0].base1 = 0x0;
  gdt[0].access = 0x0;
  gdt[0].limit1 = 0x0;
  gdt[0].flags = 0x0;
  gdt[0].base2 = 0x0;

  /* Kernel code */
  gdt[1].base0 = 0x0;
  gdt[1].limit0 = 0xfffff & 0xffff;
  gdt[1].base1 = 0x0;
  gdt[1].access = 0x9A;
  gdt[1].limit1 = 0xfffff >> 16 & 0xffff;
  gdt[1].flags = 0xA;
  gdt[1].base2 = 0x0;

  /* Kernel data */
  gdt[2].base0 = 0x0;
  gdt[2].limit0 = 0xfffff & 0xffff;
  gdt[2].base1 = 0x0;
  gdt[2].access = 0x92;
  gdt[2].limit1 = 0xfffff >> 16 & 0xffff;
  gdt[2].flags = 0xC;
  gdt[2].base2 = 0x0;

  /* Null entry */
  gdt[3].base0 = 0x0;
  gdt[3].limit0 = 0xfffff & 0xffff;
  gdt[3].base1 = 0x0;
  gdt[3].access = 0x0;
  gdt[3].limit1 = 0xfffff >> 16 & 0xffff;
  gdt[3].flags = 0x0;
  gdt[3].base2 = 0x0;

  /* User code */
  gdt[4].base0 = 0x0;
  gdt[4].limit0 = 0xfffff & 0xffff;
  gdt[4].base1 = 0x0;
  gdt[4].access = 0xFA;
  gdt[4].limit1 = 0xfffff >> 16 & 0xffff;
  gdt[4].flags = 0xA;
  gdt[4].base2 = 0x0;

  /* User data */
  gdt[5].base0 = 0x0;
  gdt[5].limit0 = 0xfffff & 0xffff;
  gdt[5].base1 = 0x0;
  gdt[5].access = 0xF2;
  gdt[5].limit1 = 0xfffff >> 16 & 0xffff;
  gdt[5].flags = 0xC;
  gdt[5].base2 = 0x0;

  /* TSS lower */
  gdt[6].base0 = tss_lower & 0xffff;
  gdt[6].limit0 = sizeof(_tss_t) & 0xffff;
  gdt[6].base1 = tss_lower >> 16 & 0xff;
  gdt[6].access = 0x89;
  gdt[6].limit1 = sizeof(_tss_t) >> 16 & 0xff;
  gdt[6].flags = 0x0;
  gdt[6].base2 = tss_lower >> 24 & 0xffff;

  /* TSS upper */
  gdt[7].base0 = tss_upper & 0xffff;
  gdt[7].limit0 = tss_upper >> 16 & 0xffff;
  gdt[7].base1 = 0x0;
  gdt[7].access = 0x0;
  gdt[7].limit1 = 0x0;
  gdt[7].flags = 0x0;
  gdt[7].base2 = 0x0;

  _gdtr_t gdtr;
  gdtr.offset = (uint64_t)&gdt;
  gdtr.size = (desc_count * sizeof(_gdt_entry_t)) - 1;

  gdt_load_descriptor(gdtr);
}