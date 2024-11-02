#pragma once

/**
 * @file logger.h
 * @author master@xiaojintao.email
 * @brief Listen others threads or processes's log infomation, then write to file.
 * @version 0.1
 * @date 2024-10-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <string>
#include <map>
#include <functional>
#include <iomanip>
#include <queue>
#include "../utils/socket/socket_p.h"
#include "../buffer/buffer.h"
#include "../utils/threadpool/threadpool.h"
#include "../utils/global/global.h"

class Logger
{
private:
    void Init();
    Logger(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer);
    Logger(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer);
    Logger() = delete;
    void operator=(const Logger&) = delete;
    ~Logger();

    /* ----- Members ----- */

    Socket_Listen m_listen2;
    Buffer& m_buffer;
    bool m_stop_operator;
    uint8_t m_log_level;
    std::map<uint8_t, char> m_log_level_symbol;

    static std::unique_ptr<Logger, std::function<void(Logger*)>> m_logger;

    void operator()();

    std::queue<std::pair<uint64_t, std::string>> m_log_queue;
    std::mutex m_queue_mutex;
    std::condition_variable m_queue_cv;
    std::unique_ptr<ThreadPool> m_thread_pool;

    void PushLogEntry(const std::pair<uint64_t, std::string>& log_entry);
    void ProcessLogEntry();

public:
    // Bind operator() and Get_Instance(), return callable object of class Logger.
    static std::function<void()> Start(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer);
    // Bind operator() and Get_Instance(), return callable object of class Logger.
    static std::function<void()> Start(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer);

    static void Stop();

    static Logger& Get_Instance();

    void Set_Log_Level(uint8_t log_level);
};