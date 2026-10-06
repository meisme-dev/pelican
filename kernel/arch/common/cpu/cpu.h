#pragma once
#include <limine/limine.h>

struct limine_smp_response *cpu_init();
void cpu_enable_features();
void cpu_halt();
