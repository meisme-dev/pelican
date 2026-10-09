#pragma once
#include <stdint.h>

#define MADT_PROCESSOR_LAPIC 0
#define MADT_IOAPIC 1
#define MADT_IOAPIC_ISO 2
#define MADT_IOAPIC_NMI 3
#define MADT_LAPIC_NMI 4
#define MADT_LAPIC_AO 5
#define MADT_PROCESSOR_LX2APIC 9

#define LAPIC_IDR 0x20
#define LAPIC_VER 0x30
#define LAPIC_TPR 0x80
#define LAPIC_APR 0x90
#define LAPIC_PPR 0xA0
#define LAPIC_EOIR 0xB0
#define LAPIC_RRD 0xC0
#define LAPIC_LDR 0xD0
#define LAPIC_DFR 0xE0
#define LAPIC_SIVR 0xF0
#define LAPIC_ISR 0x100
#define LAPIC_TMR 0x180
#define LAPIC_IRR 0x200
#define LAPIC_ESR 0x280
#define LAPIC_CMCIR 0x2F0
#define LAPIC_ICR 0x300
#define LAPIC_LVT_TIMER_R 0x320
#define LAPIC_LVT_THERMAL_R 0x330
#define LAPIC_LVT_PERF_R 0x340
#define LAPIC_LVT_LINT0_R 0x350
#define LAPIC_LVT_LINT1_R 0x360
#define LAPIC_LVT_ERR_R 0x370
#define LAPIC_TIMER_ICR 0x380
#define LAPIC_TIMER_CCR 0x390
#define LAPIC_TIMER_DCR 0x3E0

typedef struct __attribute__((packed)) {
  char sig[8];
  uint8_t checksum;
  char oem_id[6];
  uint8_t rev;
  uint32_t rsdt_addr;
  uint32_t len;
  uint64_t xsdt_addr;
  uint8_t extended_checksum;
  uint8_t reserved[3];
} xsdp_t;

typedef struct __attribute__((packed)) {
  char sig[4];
  uint32_t len;
  uint8_t rev;
  uint8_t checksum;
  char oem_id[6];
  char oem_table_id[8];
  uint32_t oem_rev;
  uint32_t creator_id;
  uint32_t creator_rev;
} acpi_sdt_header_t;

typedef struct __attribute__((packed)) {
  acpi_sdt_header_t header;
  uint64_t sdt_ptr[];
} xsdt_t;

typedef struct __attribute__((packed)) {
  acpi_sdt_header_t header;
  uint32_t addr;
  uint32_t flags;
  uint8_t records[];
} madt_t;

typedef struct __attribute__((packed)) {
  uint8_t entry_type;
  uint8_t len;
} madt_entry_header_t;

typedef struct __attribute__((packed)) {
  madt_entry_header_t header;
  uint8_t processor_id;
  uint8_t apic_id;
  uint32_t flags;
} madt_lapic_entry_t;

typedef struct __attribute__((packed)) {
  madt_entry_header_t header;
  uint8_t ioapic_id;
  uint8_t res0;
  uint32_t ioapic_addr;
  uint32_t gsi_base;
} madt_ioapic_entry_t;

typedef struct __attribute__((packed)) {
  madt_entry_header_t header;
  uint8_t bus_src;
  uint8_t irq_src;
  uint32_t gsi;
  uint16_t flags;
} madt_ioapic_iso_entry_t;

typedef struct __attribute__((packed)) {
  madt_entry_header_t header;
  uint8_t nmi_src;
  uint8_t res0;
  uint16_t flags;
  uint32_t gsi;
} madt_ioapic_nmi_entry_t;

typedef struct __attribute__((packed)) {
  madt_entry_header_t header;
  uint8_t processor_id;
  uint16_t flags;
  uint8_t lint;
} madt_lapic_nmi_entry_t;

typedef struct __attribute__((packed)) {
  madt_entry_header_t header;
  uint16_t res0;
  uint64_t lapic_addr;
} madt_lapic_ao_entry_t;

typedef struct __attribute__((packed)) {
  madt_entry_header_t header;
  uint16_t res0;
  uint32_t lapic_id;
  uint32_t flags;
  uint32_t acpi_id;
} madt_processor_lx2apic_entry_t;

void acpi_init();