#include "lock.h"
#include <common/exception/panic.h>
#include <stdint.h>

void acquire(atomic_flag *flag) {
  while (atomic_flag_test_and_set_explicit(flag, memory_order_acquire))
    ;
}

void release(atomic_flag *flag) {
  atomic_flag_clear_explicit(flag, memory_order_release);
}
