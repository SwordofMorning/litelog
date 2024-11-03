#pragma once

#include "../../../others/version/version.h"
#include "include.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Level */
enum
{
    // Silence, 0000 0000
    LOG_LEVEL_S = 0,

    // Fatal, 0000 0001
    LOG_LEVEL_F = 1 << 0,

    // Error, 0000 0010
    LOG_LEVEL_E = 1 << 1,

    // Warning, 0000 0100
    LOG_LEVEL_W = 1 << 2,

    // Notice, 0000 1000
    LOG_LEVEL_N = 1 << 3,

    // Info, 0001 0000
    LOG_LEVEL_I = 1 << 4,

    // Debug, 0010 0000
    LOG_LEVEL_D = 1 << 5,

    // Trace, 0100 0000
    LOG_LEVEL_T = 1 << 6,

    // Kernel, 1000 0000
    LOG_LEVEL_K = 1 << 7,

    // All, 1111 1111
    LOG_LEVEL_A = 0xFF
};

/* Log Level Combine */
enum
{
    // Fatal, Error, Warning, Notice, Info
    LOG_LEVEL_PRODUCTION = LOG_LEVEL_F | LOG_LEVEL_E | LOG_LEVEL_W | LOG_LEVEL_N | LOG_LEVEL_I,

    // Fatal, Error, Warning, Notice, Info, Debug, Trace
    LOG_LEVEL_DEVELOPMENT = LOG_LEVEL_PRODUCTION | LOG_LEVEL_D | LOG_LEVEL_T,

    // Fatal, Error, Warning, Notice, Info, Debug, Trace, Kernel
    LOG_LEVEL_FULL = LOG_LEVEL_A
};

/* Litelog Control Interface */
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