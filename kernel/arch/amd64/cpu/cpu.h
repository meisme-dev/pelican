#pragma once
#include <limine/limine.h>
#include <stdint.h>

uint64_t cpu_rdmsr(uint32_t msr);
void cpu_enable_sse();
void cpu_enable_avx();
uint64_t getrsp();
uint64_t cpu_get_rsp();
void cpu_wrmsr(uint32_t msr, uint32_t low, uint32_t high);
