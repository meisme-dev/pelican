#include <device/display/log.h>
#include <device/display/terminal.h>
#include <stdarg.h>

void log(_log_level_t log_level, char *format, ...) {
  va_list args;
  va_start(args, format);
  switch (log_level) {
    case VERBOSE:
      set_col(0x0, 0x777777);
      kputs("[VERBOSE] ");
      break;
    case DEBUG:
      set_col(0x0, 0xaa5555);
      kputs("[DEBUG]   ");
      break;
    case MESSAGE:
      set_col(0x0, 0x77aaff);
      kputs("[MESSAGE] ");
      break;
    case INFO:
      set_col(0x0, 0x3399ff);
      kputs("[INFO]    ");
      break;
    case SUCCESS:
      set_col(0x0, 0x44aa88);
      kputs("[SUCCESS] ");
      break;
    case FAILURE:
      set_col(0x0, 0xaa4444);
      kputs("[FAILURE] ");
      break;
    case WARNING:
      set_col(0x0, 0xffff00);
      kputs("[WARNING] ");
      break;
    case ERROR:
      set_col(0x0, 0xff4400);
      kputs("[ERROR]  ");
      break;
    case FATAL:
      set_col(0x0, 0xff0000);
      kputs("[FATAL]  ");
      break;
  }
  set_col(0x0, 0xffffff);
  vprintf(format, args);
  puts("");
  va_end(args);
}