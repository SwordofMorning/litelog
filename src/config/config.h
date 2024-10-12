#pragma once

#include "../../utils/version.h"

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    // Change Log Level
    LOG_LEVEL_CHANGE = 0,
    // Error
    LOG_LEVEL_E = 1 << 0,
    // Warning
    LOG_LEVEL_W = 1 << 1,
    // Debug
    LOG_LEVEL_D = 1 << 2,
    // Info
    LOG_LEVEL_I = 1 << 3,
    // Kernel
    LOG_LEVEL_K = 1 << 4,
};

#ifdef __cplusplus
}
#endif