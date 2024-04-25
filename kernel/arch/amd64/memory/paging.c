#include "paging.h"
#include <limine/limine.h>
#include <memory/pmm.h>
#include <stdint.h>
#include <terminal/log.h>

#define ROUND_UP(x, y) ((((x) + (y)-1) / (y)) * (y))
#define ROUND_DOWN(x, y) ((x / y) * y)
#define ROUND(x, y) ROUND_UP(x, y) - x < x - ROUND_DOWN(x, y) ? ROUND_UP(x, y) : ROUND_DOWN(x, y)

uint64_t page_level_map_4[512] __attribute__((aligned(0x1000))) = {0};

static volatile struct limine_kernel_address_request kernel_address_request = {.id = LIMINE_KERNEL_ADDRESS_REQUEST, .revision = 0};
// static volatile struct limine_hhdm_request hhdm_request = {.id = LIMINE_HHDM_REQUEST, .revision = 0};

extern uint64_t text_section_begin, text_section_end, rodata_section_begin, rodata_section_end, data_section_begin, data_section_end;

void paging_map(uint64_t src, uint64_t dst, uint16_t flags) {
  uint8_t page_level_map_4_index = dst >> 48;
  uint8_t pointer_table_index = dst >> 36;
  uint8_t page_directory_index = dst >> 24;
  uint8_t page_table_index = dst >> 12;

  uint64_t *pointer_table = (void *)page_level_map_4[page_level_map_4_index];

  if (((uint64_t)(&pointer_table) & 0x1) == 0) {
    pointer_table = (void *)pmm_alloc_page()->base;
    page_level_map_4[page_level_map_4_index] = (uint64_t)(&pointer_table) | 0x1;
  }

  uint64_t *page_directory = (void *)pointer_table[pointer_table_index];

  if (((uint64_t)(&page_directory) & 0x1) == 0) {
    page_directory = (void *)pmm_alloc_page()->base;
    pointer_table[pointer_table_index] = (uint64_t)(&page_directory) | 0x1;
  }

  uint64_t *page_table = (void *)page_directory[page_directory_index];

  if (((uint64_t)(&page_table) & 0x1) == 0) {
    page_table = (void *)pmm_alloc_page()->base;
    page_directory[page_directory_index] = (uint64_t)(&page_table) | 0x1;
  }

  page_table[page_table_index] = src | (flags & 0xFFF) | 0x01;
}

void paging_init(void) {
  int64_t offset = kernel_address_request.response->physical_base - kernel_address_request.response->virtual_base;

  uint64_t text_begin = ROUND_DOWN((uint64_t)&text_section_begin, PAGE_SIZE);
  uint64_t rodata_begin = ROUND_DOWN((uint64_t)&rodata_section_begin, PAGE_SIZE);
  uint64_t data_begin = ROUND_DOWN((uint64_t)&data_section_begin, PAGE_SIZE);
  uint64_t text_end = ROUND_UP((uint64_t)&text_section_begin, PAGE_SIZE);
  uint64_t rodata_end = ROUND_UP((uint64_t)&rodata_section_begin, PAGE_SIZE);
  uint64_t data_end = ROUND_UP((uint64_t)&data_section_begin, PAGE_SIZE);

  log_print(DEBUG, "%x, %x", text_begin, text_begin + offset);

  for (uint64_t i = text_begin; i < text_end; i += PAGE_SIZE) {
    paging_map(i + offset, i, 0x100); // Read + Supervisor
  }

  for (uint64_t i = rodata_begin; i < rodata_end; i += PAGE_SIZE) {
    paging_map(i + offset, i, 0x100); // Read + Supervisor
  }

  for (uint64_t i = data_begin; i < data_end; i += PAGE_SIZE) {
    paging_map(i + offset, i, 0x110); // Read/Write + Supervisor
  }

  log_print(DEBUG, "%x", (uint64_t)(&page_level_map_4));

  paging_load((uint64_t)(&page_level_map_4));
}
