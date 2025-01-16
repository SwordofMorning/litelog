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
#include "message.h"
#include "../sink/sink.h"

class Logger
{
private:
    void Init();
    Logger(std::vector<std::unique_ptr<ISink>> sinks, Buffer& buffer);
    Logger() = delete;
    void operator=(const Logger&) = delete;
    ~Logger();

    /* ----- Members ----- */
    std::vector<std::unique_ptr<ISink>> m_sinks;
    Buffer& m_buffer;
    bool m_stop_operator;
    uint8_t m_log_level;
    std::map<uint8_t, char> m_log_level_symbol;
    uint64_t m_log_id;

    static std::unique_ptr<Logger, std::function<void(Logger*)>> m_logger;

    void operator()();

    std::queue<Message> m_log_queue;
    std::mutex m_queue_mutex;
    std::condition_variable m_queue_cv;
    std::unique_ptr<ThreadPool> m_thread_pool;

    void PushLogEntry(const Message& msg);
    void ProcessLogEntry();

public:
    static std::function<void()> Start(std::vector<std::unique_ptr<ISink>> sinks, Buffer& buffer);
    static void Stop();
    static Logger& Get_Instance();
    void Set_Log_Level(uint8_t log_level);
};