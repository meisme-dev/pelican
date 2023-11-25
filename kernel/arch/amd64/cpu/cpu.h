#pragma once
#include <limine/limine.h>
#include <stdint.h>

uint64_t rdmsr(uint32_t msr);
void wrmsr(uint32_t msr, uint32_t low, uint32_t high);
