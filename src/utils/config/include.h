#pragma once

#include "../../../others/version/version.h"
#include "include.h"

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    // Silence
    LOG_LEVEL_S = 0,
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
    // Litelog itself
    LOG_LEVEL_L = 1 << 5,
    // All
    LOG_LEVEL_A = 1 << 6,
};

enum
{
    CTL_STOP_PROGRAM = 0,
    CTL_CHANGE_LEVEL,
    CTL_SWITCH_PAGE,
};

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#ifdef __cplusplus
}
#endif