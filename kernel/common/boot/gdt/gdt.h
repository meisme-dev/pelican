#pragma once

#if defined(amd64)
#include <arch/amd64/boot/gdt/gdt.h>
#else
#error "This architecture is not supported!"
#endif