#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <sync/lock.h>
#include <terminal/log.h>
#include <terminal/terminal.h>

#define LOG_LEVEL_INFO(log_level, color) \
  {log_level, #log_level, color}

static uint8_t level = 4;
static struct log_level_info {
  log_level_t log_level;
  char *name;
  uint32_t color;
} infos[] = {
    LOG_LEVEL_INFO(SYSTEM, 0xaa5555),
    LOG_LEVEL_INFO(INFO, 0x3399ff),
    LOG_LEVEL_INFO(OK, 0x44aa88),
    LOG_LEVEL_INFO(FAIL, 0xaa4444),
    LOG_LEVEL_INFO(WARN, 0xffff00),
    LOG_LEVEL_INFO(HALT, 0xff0000)};

void log_init(uint8_t log_level) {
  level = log_level;
  log_print(OK, "Initialized logger");
}

void log_print(log_level_t log_level, char *format, ...) {
  static atomic_flag lock = ATOMIC_FLAG_INIT;
  acquire(&lock);
  va_list args;
  va_start(args, format);
  if (log_level < level) {
    release(&lock);
    return;
  }
  for (int32_t i = sizeof(infos) / sizeof(struct log_level_info); i--;) {
    if (infos[i].log_level == log_level) {
      uint8_t info_len = strlen(infos[i].name);
      uint8_t space_count = 6 - info_len;
      uint32_t bg = 0, fg = 0;
      bool bold = false;

      get_defaults(&bg, &fg, &bold);

      set_bold(true);
      kputchar('[');

      for (uint8_t j = 0; j < space_count / 2; j++) {
        kputchar(' ');
      }

      set_color(bg, infos[i].color);
      kputs(infos[i].name);
      set_color(bg, fg);

      for (uint8_t j = 0; j < space_count / 2; j++) {
        kputchar(' ');
      }

      kputs("] ");
      set_bold(bold);
    }
  }
  vprintf(format, args);
  puts("");
  va_end(args);
  release(&lock);
}

#undef LOG_LEVEL_INFO
