#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================================== */
/* ======================================== Define ======================================== */
/* ======================================================================================== */

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

enum
{
    CTL_STOP_PROGRAM = 0,
    CTL_CHANGE_LEVEL,
    CTL_SWITCH_PAGE,
};

/* ======================================================================================== */
/* ======================================== Socket ======================================== */
/* ======================================================================================== */

struct Socket_Wrap
{
    // socket device.
    int device;
    // this socket's ip and port.
    struct sockaddr_in self_address;
};

/**
 * @brief Create device descriptor for socket wrap.
 * 
 * @param p_socket 
 * @return success or fail.
 * @retval 0, success.
 * @retval -1, socket open device fail.
 */
static int Socket_Open_Device(struct Socket_Wrap* p_socket)
{
    p_socket->device = socket(AF_INET, SOCK_DGRAM, 0);

    return p_socket->device == -1 ? -1 : 0;
}

/**
 * @brief Bind ip and port for one socket.
 * 
 * @param p_socket socket wrap.
 * @param p_self_ip ip of itself.
 * @param p_self_port port of itself.
 * @return int 
 */
static int Socket_Bind_Target(struct Socket_Wrap* p_socket, const char* p_self_ip, uint16_t p_self_port)
{
    int retval = 0;

    p_socket->self_address.sin_family = AF_INET;
    p_socket->self_address.sin_addr.s_addr = inet_addr(p_self_ip);
    p_socket->self_address.sin_port = htons(p_self_port);

    socklen_t slen = sizeof(p_socket->self_address);

    if (-1 == bind(p_socket->device, (struct sockaddr*)&(p_socket->self_address), slen))
    {
        perror("bind fail");
        close(p_socket->device);
        retval = -1;
    }

    return retval;
}

int Socket_Init(struct Socket_Wrap* p_socket, const char* p_self_ip, uint16_t p_self_port)
{
    int retval = 0;

    if (Socket_Open_Device(p_socket) != 0)
    {
        retval = -1;
        goto out_return;
    }

    if (Socket_Bind_Target(p_socket, p_self_ip, p_self_port) != 0)
    {
        retval = -2;
        goto out_return;
    }

out_return:
    return retval;
}

void Socket_Create_Target(struct sockaddr_in* p_target, const char* p_target_ip, uint16_t p_target_port)
{
    (*p_target).sin_family = AF_INET;
    (*p_target).sin_addr.s_addr = inet_addr(p_target_ip);
    (*p_target).sin_port = htons(p_target_port);
}

int Socket_Send(int device, uint8_t* buffer, size_t n, struct sockaddr* target)
{
    socklen_t length = sizeof(*target);
    return sendto(device, buffer, n, MSG_CONFIRM, target, length);
}

int Socket_Recv(int device, uint8_t* buffer, size_t n, struct sockaddr* target, int timeout_ms)
{
    fd_set rfds;
    struct timeval tv;
    struct timeval* ptv = NULL;
    int selectRet;

    if (timeout_ms > 0)
    {
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;
        ptv = &tv;
    }

    FD_ZERO(&rfds);
    FD_SET(device, &rfds);

    selectRet = select(device + 1, &rfds, NULL, NULL, ptv);

    // Select ret error
    if (selectRet == -1)
    {
        return -1;
    }
    // Timeout
    else if (selectRet == 0)
    {
        errno = EAGAIN;
        return -2;
    }
    // Return data
    else
    {
        socklen_t length = sizeof(*target);
        return recvfrom(device, (char*)buffer, n, MSG_WAITALL, target, &length);
    }
}

void Socket_Exit(struct Socket_Wrap* p_socket)
{
    if (p_socket->device != -1)
    {
        close(p_socket->device);
        p_socket->device = -1;
    }
}

/* ======================================================================================== */
/* ======================================== Client ======================================== */
/* ======================================================================================== */

struct Socket_Wrap local;
// local of monitor
struct sockaddr_in monitor;
// local of controller
struct sockaddr_in controller;

static char program_name[32] = {0};

void Litelog_Init(const char* p_program_name)
{
    char* local_ip = "127.0.0.1";
    uint16_t local_port = 50000;

    char* monitor_ip = "127.0.0.1";
    uint16_t monitor_port = 20000;

    char controller_ip[] = "127.0.0.1";
    uint16_t controller_port = 20001;

    Socket_Init(&local, local_ip, local_port);
    Socket_Create_Target(&monitor, monitor_ip, monitor_port);
    Socket_Create_Target(&controller, controller_ip, controller_port);

    snprintf(program_name, sizeof(program_name), "[%s]", p_program_name);
}

void Litelog_Exit()
{
    Socket_Exit(&local);
}

int Litelog_Send(uint8_t* buffer, size_t n, struct sockaddr_in target)
{
    return Socket_Send(local.device, buffer, n, (struct sockaddr*)&target);
}

int Litelog_Log(uint8_t level, const char* str, size_t n)
{
    int ret = 0;

    // Valid Check
    uint8_t valid_levels = ~LOG_LEVEL_K & 0xFF;
    if ((level & ~valid_levels) != 0 || (level & (level - 1)) != 0)
    {
        ret = -1;
        goto out_return;
    }

    // 计算程序名长度（包括方括号）
    size_t program_name_len = strlen(program_name);
    
    // Malloc Buffer: level(1) + program_name + content
    uint8_t* buffer = (uint8_t*)malloc(1 + program_name_len + n);
    if (buffer == NULL)
    {
        ret = -2;
        goto out_return;
    }

    // 填充数据
    buffer[0] = level;                                    // 日志级别
    memcpy(buffer + 1, program_name, program_name_len);  // 程序名
    memcpy(buffer + 1 + program_name_len, str, n);       // 日志内容

    ret = Litelog_Send(buffer, 1 + program_name_len + n, monitor);

    free(buffer);

out_return:
    return ret;
}

int Litelog_Log_Precise (uint8_t level, const char* file, int line, const char* func, const char* format, ...)
{
    int ret = 0;

    // Valid Check
    uint8_t valid_levels = ~LOG_LEVEL_K & 0xFF;
    if ((level & ~valid_levels) != 0 || (level & (level - 1)) != 0)
    {
        ret = -1;
        goto out_return;
    }

    // Create Log Message
    char log_buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(log_buffer, sizeof(log_buffer), format, args);
    va_end(args);

    // Get File Name
    const char* file_name = strrchr(file, '/') ? strrchr(file, '/') + 1 : file;

    // Format Log Message
    char formatted_log[512];
    snprintf(formatted_log, sizeof(formatted_log), "%s:%d %s: %s", 
             file_name, line, func, log_buffer);

    // 计算长度
    size_t program_name_len = strlen(program_name);
    size_t log_length = strlen(formatted_log);
    
    // Malloc Buffer
    uint8_t* buffer = (uint8_t*)malloc(1 + program_name_len + log_length);
    if (buffer == NULL)
    {
        ret = -2;
        goto out_return;
    }

    // 填充数据
    buffer[0] = level;                                    // 日志级别
    memcpy(buffer + 1, program_name, program_name_len);  // 程序名
    memcpy(buffer + 1 + program_name_len,                // 日志内容
           formatted_log, log_length);

    ret = Litelog_Send(buffer, 1 + program_name_len + log_length, monitor);

    free(buffer);

out_return:
    return ret;
}

int Litelog_Shutdown()
{
    uint8_t command[1] = {CTL_STOP_PROGRAM};
    return Litelog_Send(command, 1, controller);
}

int Litelog_Change_Level(uint8_t level)
{
    uint8_t command[2] = {CTL_CHANGE_LEVEL, level};
    return Litelog_Send(command, 2, controller);
}

int Litelog_Switch_Page()
{
    uint8_t command[1] = {CTL_SWITCH_PAGE};
    return Litelog_Send(command, 1, controller);
}

/* ===================================================================================== */
/* ======================================== API ======================================== */
/* ===================================================================================== */

void Litelog_Log_Fatal(const char* format, ...)
{
    char log_buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(log_buffer, sizeof(log_buffer), format, args);
    va_end(args);
    Litelog_Log(LOG_LEVEL_F, log_buffer, strlen(log_buffer));
}

void Litelog_Log_Error(const char* format, ...)
{
    char log_buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(log_buffer, sizeof(log_buffer), format, args);
    va_end(args);
    Litelog_Log(LOG_LEVEL_E, log_buffer, strlen(log_buffer));
}

void Litelog_Log_Warning(const char* format, ...)
{
    char log_buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(log_buffer, sizeof(log_buffer), format, args);
    va_end(args);
    Litelog_Log(LOG_LEVEL_W, log_buffer, strlen(log_buffer));
}

void Litelog_Log_Notice(const char* format, ...)
{
    char log_buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(log_buffer, sizeof(log_buffer), format, args);
    va_end(args);
    Litelog_Log(LOG_LEVEL_N, log_buffer, strlen(log_buffer));
}

void Litelog_Log_Info(const char* format, ...)
{
    char log_buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(log_buffer, sizeof(log_buffer), format, args);
    va_end(args);
    Litelog_Log(LOG_LEVEL_I, log_buffer, strlen(log_buffer));
}

void Litelog_Log_Debug(const char* format, ...)
{
    char log_buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(log_buffer, sizeof(log_buffer), format, args);
    va_end(args);
    Litelog_Log(LOG_LEVEL_D, log_buffer, strlen(log_buffer));
}

void Litelog_Log_Trace(const char* format, ...)
{
    char log_buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(log_buffer, sizeof(log_buffer), format, args);
    va_end(args);
    Litelog_Log(LOG_LEVEL_T, log_buffer, strlen(log_buffer));
}

struct LitelogLevel
{
    void (*fatal)(const char* format, ...);
    void (*error)(const char* format, ...);
    void (*warning)(const char* format, ...);
    void (*notice)(const char* format, ...);
    void (*info)(const char* format, ...);
    void (*debug)(const char* format, ...);
    void (*trace)(const char* format, ...);
    int (*precise)(uint8_t level, const char* file, int line, const char* func, const char* format, ...);
};

struct Litelog
{
    void (*init)(const char* p_program_name);
    void (*exit)();
    int (*shutdown)();
    int (*change_level)(uint8_t level);
    int (*switch_page)();
    struct LitelogLevel log;
};

// clang-format off
struct Litelog litelog =
{
    .init = Litelog_Init,
    .exit = Litelog_Exit,
    .shutdown = Litelog_Shutdown,
    .change_level = Litelog_Change_Level,
    .switch_page = Litelog_Switch_Page,
    .log = {
        .fatal = Litelog_Log_Fatal,
        .error = Litelog_Log_Error,
        .warning = Litelog_Log_Warning,
        .notice = Litelog_Log_Notice,
        .info = Litelog_Log_Info,
        .debug = Litelog_Log_Debug,
        .trace = Litelog_Log_Trace,
        .precise  = Litelog_Log_Precise 
    }
};
// clang-format on

#ifdef __cplusplus
}
#endif