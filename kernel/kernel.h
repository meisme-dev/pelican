#pragma once

#include <arch/common/device/pci/pci.h>
#include <memory/pmm.h>

#define ROUND_UP(x, y) ((((x) + (y)-1) / (y)) * (y))
#define ROUND_DOWN(x, y) ((x / y) * y)
#define ROUND(x, y) ROUND_UP(x, y) - x < x - ROUND_DOWN(x, y) ? ROUND_UP(x, y) : ROUND_DOWN(x, y)

#ifndef LOGLEVEL
#define LOGLEVEL DEBUG
#endif
