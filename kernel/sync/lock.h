#pragma once
#include <stdatomic.h>

void acquire(atomic_flag *flag);
void release(atomic_flag *flag);
