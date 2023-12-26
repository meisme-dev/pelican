#pragma once
#include <stdatomic.h>

void acquire(volatile atomic_flag *flag);
void release(volatile atomic_flag *flag);
