#include "acpi.h"
#include "terminal/log.h"
#include <exception/panic.h>
#include <limine/limine.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 6,
};

rsdp_t *rsdp_init() {
  void *addr = rsdp_request.response->address;
  volatile uint32_t sum = 0;

  uint32_t len = ((xsdp_t *)addr)->len;

  for (uint32_t i = 0; i < len; i++) {
    sum += ((char *)addr)[i];
  }

  if (strncmp("RSD PTR", ((xsdp_t *)addr)->rsdp.sig, 7)) {
    panic("Invalid XSDP/RSDP, signature %.4s", ((xsdp_t *)addr)->rsdp.sig);
  }

  if ((sum & 0xFF) != 0) {
    panic("Invalid XSDP/RSDP, checksum %x", sum & 0xFF);
  }

  return rsdp_request.response->address;
}

static acpi_sdt_header_t *find_sdt(rsdt_t *rsdt, const char *sig, uint8_t rev) {
  uint8_t size = rev >= 2 ? 8 : 4;

  uint32_t entries = (rsdt->header.len - sizeof(rsdt->header)) / size;

  for (uint32_t i = 0; i < entries; i++) {
    acpi_sdt_header_t *header = rev >= 2 ? (acpi_sdt_header_t *)((xsdt_t *)rsdt)->sdt_ptr[i] : (acpi_sdt_header_t *)(uintptr_t)(rsdt)->sdt_ptr[i];
    if (!strncmp(sig, header->sig, 4)) {
      log_print(SYSTEM, "SDT found with signature %.4s", header->sig);
      return header;
    }
  }

  return NULL;
}

void acpi_init() {
  rsdp_t *rsdp = rsdp_init();
  madt_t *madt = NULL;

  if (rsdp->rev >= 2) {
    xsdp_t *xsdp = (void *)rsdp;
    xsdt_t *xsdt = (void *)xsdp->xsdt_addr;
    madt = (void *)find_sdt((rsdt_t *)xsdt, "APIC", rsdp->rev);
  } else {
    rsdt_t *rsdt = (void *)(uint64_t)rsdp->rsdt_addr;
    madt = (void *)find_sdt(rsdt, "APIC", rsdp->rev);
  }

  void *lapic = NULL;
  uint8_t *end = (uint8_t *)madt + madt->header.len;
  for (uint8_t *cur = madt->records; cur < end; cur += cur[1]) {
    switch (cur[0]) {
      case MADT_LAPIC_AO:
        lapic = (void *)((madt_lapic_ao_entry_t *)cur)->lapic_addr;
    }
  }
}