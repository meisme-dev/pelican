#pragma once

typedef enum {
  VERBOSE,
  DEBUG,
  MESSAGE,
  INFO,
  SUCCESS,
  FAILURE,
  WARNING,
  ERROR,
  FATAL
} _log_level_t;

void log(_log_level_t log_level, char *format, ...);
