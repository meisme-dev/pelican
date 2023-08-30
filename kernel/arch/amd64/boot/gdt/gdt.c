#include "gdt.h"
#include <string.h>
#include <terminal/log.h>

#define DESC_COUNT 8

// MUST BE GLOBAL OR WILL BE DROPPED
_tss_t tss = {0};
_gdt_entry_t gdt[DESC_COUNT];

static void gdt_set_entry(_gdt_entry_t *entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
  entry->base0 = base & 0xffff;
  entry->limit0 = limit & 0xffff;
  entry->base1 = base >> 16 & 0xff;
  entry->access = access;
  entry->limit1 = limit >> 16 & 0xf;
  entry->flags = flags & 0xf;
  entry->base2 = base >> 24 & 0xff;
}

static void gdt_set_tss(_gdt_entry_t *entries) {
  entries[0].base0 = sizeof(tss) - 1;
  entries[0].limit0 = (uint64_t)&tss & 0xffff;
  entries[0].base1 = ((uint64_t)&tss >> 16) & 0xff;
  entries[0].access = 0x89;
  entries[0].limit1 = 0x0;
  entries[0].flags = 0x0;
  entries[0].base2 = ((uint64_t)&tss >> 24) & 0xff;

  entries[1].base0 = ((uint64_t)&tss >> 32) & 0xffff;
  entries[1].limit0 = ((uint64_t)&tss >> 48) & 0xffff;
  entries[1].base1 = 0x0;
  entries[1].access = 0x0;
  entries[1].limit1 = 0x0;
  entries[1].flags = 0x0;
  entries[1].base2 = 0x0;
}

void gdt_init() {
  tss.iopb = sizeof(_tss_t);

  gdt_set_entry(&gdt[0], 0x0, 0x0, 0x0, 0x0);                              // Null
  gdt_set_entry(&gdt[1], 0x0, 0x0, 0x9A, 0xA);                             // Kernel code
  gdt_set_entry(&gdt[2], 0x0, 0x0, 0x92, 0xC);                             // Kernel data
  gdt_set_entry(&gdt[3], 0x0, 0x0, 0x0, 0x0);                              // Null
  gdt_set_entry(&gdt[4], 0x0, 0x0, 0xFA, 0xA);                             // User code
  gdt_set_entry(&gdt[5], 0x0, 0x0, 0xF2, 0xC);                             // User data
  gdt_set_tss(&gdt[6]);                                                    // TSS
  gdt_load((DESC_COUNT * sizeof(_gdt_entry_t)) - 1, (uint64_t)&gdt, 0x30); // 0x30 is the 6th entry
  gdt_segments_reload();
  // log(SUCCESS, "Loaded GDT at 0x%x", (uint64_t)&gdt);
  // log(SUCCESS, "Loaded TSS at 0x%x", (uint64_t)&tss);
}

#undef DESC_COUNT