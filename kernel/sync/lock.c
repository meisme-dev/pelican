#include "lock.h"
#include <common/exception/panic.h>
#include <stdatomic.h>
#include <stdint.h>

static atomic_flag used = ATOMIC_FLAG_INIT;

void lock() {
  while (atomic_flag_test_and_set_explicit(&used, memory_order_acquire));
}

void unlock() {
  atomic_flag_clear_explicit(&used, memory_order_release);
}
