#if defined(amd64)
#include <arch/amd64/exception/idt.h>
#else
#error "This architecture is not supported!"
#endif