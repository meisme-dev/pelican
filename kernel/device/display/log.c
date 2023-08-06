#include <device/display/log.h>
#include <device/display/terminal.h>
#include <stdarg.h>

void log(_log_level_t log_level, char *format, ...) {
  va_list args;
  va_start(args, format);
  switch (log_level) {
    case VERBOSE:
      set_col(0x0, 0x777777);
      printf("[VERBOSE] ");
      set_col(0x0, 0xffffff);
      vprintf(format, args);
      puts("");
      break;
    case DEBUG:
      set_col(0x0, 0xaa5555);
      printf("[DEBUG] ");
      set_col(0x0, 0xffffff);
      vprintf(format, args);
      puts("");
      break;
    case MESSAGE:
      set_col(0x0, 0x77aaff);
      printf("[MESSAGE] ");
      set_col(0x0, 0xffffff);
      vprintf(format, args);
      puts("");
      break;
    case INFO:
      set_col(0x0, 0x3399ff);
      printf("[INFO] ");
      set_col(0x0, 0xffffff);
      vprintf(format, args);
      puts("");
      break;
    case SUCCESS:
      set_col(0x0, 0x44aa88);
      printf("[SUCCESS] ");
      set_col(0x0, 0xffffff);
      vprintf(format, args);
      puts("");
      break;
    case FAILURE:
      set_col(0x0, 0xaa4444);
      printf("[FAILURE] ");
      set_col(0x0, 0xffffff);
      vprintf(format, args);
      puts("");
      break;
    case WARNING:
      set_col(0x0, 0xffff00);
      printf("[WARNING] ");
      set_col(0x0, 0xffffff);
      vprintf(format, args);
      puts("");
      break;
    case ERROR:
      set_col(0x0, 0xff4400);
      printf("[ERROR] ");
      set_col(0x0, 0xffffff);
      vprintf(format, args);
      puts("");
      break;
    case FATAL:
      set_col(0x0, 0xff0000);
      printf("[FATAL] ");
      set_col(0x0, 0xffffff);
      vprintf(format, args);
      puts("");
      break;
  }
  va_end(args);
}