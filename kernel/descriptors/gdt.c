#include <descriptors/gdt.h>
#include <device/display/terminal.h>

static void encode_entry(gdt_descriptor descriptor, Gdt gdt) {
  descriptor[0] = gdt.gdt_s.limit & 0xFF;
  descriptor[1] = (gdt.gdt_s.limit >> 8) & 0xFF;
  descriptor[6] = (gdt.gdt_s.limit >> 16) & 0x0F;
  descriptor[2] = gdt.gdt_s.base & 0xFF;
  descriptor[3] = (gdt.gdt_s.base >> 8) & 0xFF;
  descriptor[4] = (gdt.gdt_s.base >> 16) & 0xFF;
  descriptor[7] = (gdt.gdt_s.base >> 24) & 0xFF;
  descriptor[5] = gdt.gdt_s.access;
  descriptor[6] |= (gdt.gdt_s.flags << 4);
}

static void create_descriptors(Gdt *gdt, gdt_descriptor *descriptors, size_t num) {
  for (size_t i = 0; i < num; i++) {
    encode_entry(descriptors[i], gdt[i]);
  }
}

static void load_descriptor(Gdtr gdtr) {
    __asm__ volatile("lgdt %0":: "m"(gdtr) : "memory");
}

void init_gdt() {
    asm("cli");

    const uint8_t desc_count = 8;
    Tss *tss = {(Tss *)NULL};
    uint32_t tss_lower = (uint64_t)tss & 0xffffffff;
    uint32_t tss_upper = (uint64_t)tss >> 32;
    gdt_descriptor descriptors[desc_count];
    Gdt gdt[desc_count];

    /* Null entry */
    gdt[0].gdt_s.base = 0x0;
    gdt[0].gdt_s.limit = 0x0;
    gdt[0].gdt_s.access = 0x0;
    gdt[0].gdt_s.flags = 0x0;

    /* Kernel code */
    gdt[1].gdt_s.base = 0x0;
    gdt[1].gdt_s.limit = 0xFFFFF;
    gdt[1].gdt_s.access = 0x9A;
    gdt[1].gdt_s.flags = 0xA;

    /* Kernel data */
    gdt[2].gdt_s.base = 0x0;
    gdt[2].gdt_s.limit = 0xFFFFF;
    gdt[2].gdt_s.access = 0x92;
    gdt[2].gdt_s.flags = 0xC;

    /* Null entry */
    gdt[3].gdt_s.base = 0x0;
    gdt[3].gdt_s.limit = 0x0;
    gdt[3].gdt_s.access = 0x0;
    gdt[3].gdt_s.flags = 0x0;

    /* User code */
    gdt[4].gdt_s.base = 0x0;
    gdt[4].gdt_s.limit = 0xFFFFF;
    gdt[4].gdt_s.access = 0xFA;
    gdt[4].gdt_s.flags = 0xA;

    /* User data */
    gdt[5].gdt_s.base = 0x0;
    gdt[5].gdt_s.limit = 0xFFFFF;
    gdt[5].gdt_s.access = 0xF2;
    gdt[5].gdt_s.flags = 0xC;

    /* TSS lower */
    gdt[6].gdt_s.base = tss_lower;
    gdt[6].gdt_s.limit = sizeof(Tss);
    gdt[6].gdt_s.access = 0x89;
    gdt[6].gdt_s.flags = 0x0;

    create_descriptors(gdt, descriptors, desc_count);

    descriptors[7][0] = tss_upper & 0xff;
    descriptors[7][1] = (tss_upper >> 8) & 0xff;
    descriptors[7][2] = (tss_upper >> 16) & 0xff;
    descriptors[7][3] = (tss_upper >> 24) & 0xff;

    Gdtr gdtr;
    gdtr.offset = (uint64_t)&descriptors;
    gdtr.size = (desc_count * sizeof(uint64_t)) - 1;

   load_descriptor(gdtr);
}