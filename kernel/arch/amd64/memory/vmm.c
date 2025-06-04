#include "vmm.h"
#include <arch/amd64/cpu/cpu.h>
#include <exception/panic.h>
#include <kernel.h>
#include <limine/limine.h>
#include <memory/pmm.h>
#include <stddef.h>
#include <stdint.h>
#include <sync/lock.h>
#include <terminal/log.h>

static uint64_t direct_map_base = 0;

static volatile struct limine_kernel_address_request kernel_address_request = {.id = LIMINE_KERNEL_ADDRESS_REQUEST, .revision = 0};
static volatile struct limine_hhdm_request hhdm_request = {.id = LIMINE_HHDM_REQUEST, .revision = 0};

extern volatile uint64_t text_section_begin, text_section_end, rodata_section_begin, rodata_section_end, data_section_begin, data_section_end;

uint64_t vmm_get_direct_map_base() {
  return direct_map_base;
}

static uint64_t *vmm_next_level(uintptr_t **current_level, uint64_t index) {
  uint64_t *next_level = 0;

  if (((*current_level)[index] & 0xFFFFFFFFFF000) == 0) {
    page_descriptor_t *page = pmm_alloc_page();

    if (page == NULL || page->base == 0) {
      panic("OUT OF MEMORY");
    }

    next_level = (void *)page->base;
    (*current_level)[index] = (uint64_t)next_level | 0b11;

    return next_level;
  }

  next_level = (void *)(((*current_level)[index] & 0xFFFFFFFFFF000) + direct_map_base);
  return next_level;
}

void vmm_map(size_t src, size_t dst, size_t flags, uintptr_t **page_map_level_4) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);

  uint64_t page_map_level_4_index = (dst >> 39) & 0x1FF;
  uint64_t pointer_table_index = (dst >> 30) & 0x1FF;
  uint64_t page_directory_index = (dst >> 21) & 0x1FF;
  uint64_t page_table_index = (dst >> 12) & 0x1FF;

  uint64_t *pointer_table = vmm_next_level(page_map_level_4, page_map_level_4_index);
  uint64_t *page_directory = vmm_next_level(&pointer_table, pointer_table_index);
  uint64_t *page_table = vmm_next_level(&page_directory, page_directory_index);

  page_table[page_table_index] = (src) | (flags);

  release(&lock);
}

uintptr_t *vmm_init(void) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);

  struct limine_kernel_address_response *kernel_address_response = kernel_address_request.response;
  struct limine_hhdm_response *hhdm_response = hhdm_request.response;

  direct_map_base = hhdm_response->offset;

  page_descriptor_t *page = pmm_alloc_page();

  if (page == NULL || page->base == 0) {
    panic("OUT OF MEMORY");
  }

  uint64_t *page_map_level_4 = (void *)(page->base + direct_map_base);

  uintptr_t text_begin = ROUND_DOWN((uintptr_t)&text_section_begin, PAGE_SIZE);
  uintptr_t rodata_begin = ROUND_DOWN((uintptr_t)&rodata_section_begin, PAGE_SIZE);
  uintptr_t data_begin = ROUND_DOWN((uintptr_t)&data_section_begin, PAGE_SIZE);
  uintptr_t text_end = ROUND_UP((uintptr_t)&text_section_end, PAGE_SIZE);
  uintptr_t rodata_end = ROUND_UP((uintptr_t)&rodata_section_end, PAGE_SIZE);
  uintptr_t data_end = ROUND_UP((uintptr_t)&data_section_end, PAGE_SIZE);

  for (size_t i = text_begin; i < text_end; i += PAGE_SIZE) {
    vmm_map(i - kernel_address_response->virtual_base + kernel_address_response->physical_base, i, 0b01, &page_map_level_4); // Read + Present
  }

  for (size_t i = rodata_begin; i < rodata_end; i += PAGE_SIZE) {
    vmm_map(i - kernel_address_response->virtual_base + kernel_address_response->physical_base, i, 0b01, &page_map_level_4); // Read + Present
  }

  for (size_t i = data_begin; i < data_end; i += PAGE_SIZE) {
    vmm_map(i - kernel_address_response->virtual_base + kernel_address_response->physical_base, i, 0b11, &page_map_level_4); // Read/Write + Present
  }

  struct limine_memmap_response *response = pmm_get_memmap();

  for (size_t i = 0; i < response->entry_count; i++) {
    struct limine_memmap_entry *current_entry = response->entries[i];

    if (current_entry->type != LIMINE_MEMMAP_USABLE &&
        current_entry->type != LIMINE_MEMMAP_FRAMEBUFFER &&
        current_entry->type != LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE) {
      continue;
    }

    for (size_t j = 0; j <= current_entry->length - PAGE_SIZE; j += PAGE_SIZE) {
      vmm_map(current_entry->base + j, current_entry->base + j, 0b11, &page_map_level_4);
      vmm_map(current_entry->base + j, current_entry->base + j + direct_map_base, 0b11, &page_map_level_4);
    }
  }

  release(&lock);

  return page_map_level_4;
}
