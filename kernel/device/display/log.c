#include <device/display/log.h>
#include <device/display/terminal.h>
#include <stdarg.h>

#define LOG_LEVEL_INFO(log_level, color) \
  { log_level, "[" #log_level "] ", color }

static uint8_t level = 4;
static struct log_level_info {
  _log_level_t log_level;
  char *name;
  uint32_t color;
} infos[] = {
    LOG_LEVEL_INFO(VERBOSE, 0x777777),
    LOG_LEVEL_INFO(DEBUG, 0xaa5555),
    LOG_LEVEL_INFO(MESSAGE, 0x77aaff),
    LOG_LEVEL_INFO(INFO, 0x3399ff),
    LOG_LEVEL_INFO(SUCCESS, 0x44aa88),
    LOG_LEVEL_INFO(FAILURE, 0xaa4444),
    LOG_LEVEL_INFO(WARNING, 0xffff00),
    LOG_LEVEL_INFO(ERROR, 0xff4400),
    LOG_LEVEL_INFO(FATAL, 0xff0000)};

void log_init(uint8_t log_level) {
  level = log_level;
  log(SUCCESS, "Initialized logging");
}

void log(_log_level_t log_level, char *format, ...) {
  va_list args;
  va_start(args, format);
  if (log_level < level) {
    return;
  }
  for (uint32_t i = 0; i < sizeof(infos) / sizeof(struct log_level_info); i++) {
    if (infos[i].log_level == log_level) {
      set_col(0x0, infos[i].color);
      kputs(infos[i].name);
    }
  }
  set_col(0x0, 0xffffff);
  vprintf(format, args);
  puts("");
  va_end(args);
}

#undef LOG_LEVEL_INFO