#pragma once

#include "../../others/version/version.h"

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
};

enum
{
    /**
     * @brief Change log level.
     * @note When Monitor receive this index as command_buffer[0], set Monitor::m_log_level to command_buffer[1].
     */
    LOG_CTL_LEVEL_CHANGE = 1 << 5,
    /**
     * @brief Exit whole program.
     * @note When Monitor receive this index as command_buffer[0], exit whole program.
     */
    LOG_CTL_EXIT = 1 << 6,
};

#ifdef __cplusplus
}
#endif