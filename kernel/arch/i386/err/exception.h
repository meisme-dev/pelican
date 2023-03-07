#ifndef EXCEPTION_H
#define EXCEPTION_H
void _panic(const char *msg, const char *file, int line);
#define panic(msg) _panic(msg, __FILE__, __LINE__)
#endif
