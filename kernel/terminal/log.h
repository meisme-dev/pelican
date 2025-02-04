#pragma once

#include <stdint.h>

typedef enum {
  DEBUG,
  VERBOSE,
  MESSAGE,
  INFO,
  SUCCESS,
  FAILURE,
  WARNING,
  ERROR,
  FATAL
} log_level_t;

void log_init(uint8_t log_level);
void log_print(log_level_t log_level, char *format, ...);
