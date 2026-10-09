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

xsdp_t *xsdp_init() {
  void *addr = rsdp_request.response->address;
  volatile uint32_t sum = 0;
  uint32_t len = ((xsdp_t *)addr)->len;

  for (uint32_t i = 0; i < len; i++) {
    sum += ((char *)addr)[i];
  }

  if (strncmp("RSD PTR", ((xsdp_t *)addr)->sig, 7)) {
    panic("Invalid XSDP, signature %s", ((xsdp_t *)addr)->sig);
  }

  if ((sum & 0xFF) != 0) {
    panic("Invalid XSDP, checksum %x", sum & 0xFF);
  }

  return rsdp_request.response->address;
}

static void *find_sdt(xsdt_t *xsdt, const char *sig) {
  uint32_t entries = (xsdt->header.len - sizeof(xsdt->header)) / 8;
  for (uint32_t i = 0; i < entries; i++) {
    if (!strncmp(sig, ((acpi_sdt_header_t *)xsdt->sdt_ptr[i])->sig, 4)) {
      log_print(SYSTEM, "SDT found with signature %s", ((acpi_sdt_header_t *)xsdt->sdt_ptr[i])->sig);
      return (void *)xsdt->sdt_ptr[i];
    }
  }
  return NULL;
}

void acpi_init() {
  xsdp_t *xsdp = xsdp_init();
  xsdt_t *xsdt = (void *)xsdp->xsdt_addr;
  madt_t *madt = find_sdt(xsdt, "APIC");
  void *lapic = (void *)((uintptr_t)madt->addr);
  uint8_t *end = (uint8_t *)madt + madt->header.len;
  for (uint8_t *cur = madt->records; cur < end; cur += cur[1]) {
    switch (cur[0]) {
      case MADT_LAPIC_AO:
        lapic = (void *)((madt_lapic_ao_entry_t *)cur)->lapic_addr;
    }
  }
}