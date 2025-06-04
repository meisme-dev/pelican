#include "lock.h"
#include <exception/panic.h>

void acquire(volatile atomic_flag *flag) {
  while (atomic_flag_test_and_set_explicit(flag, memory_order_acquire)) {
    __asm__("pause");
  }
}

void release(volatile atomic_flag *flag) {
  atomic_flag_clear_explicit(flag, memory_order_release);
}
