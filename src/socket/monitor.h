#pragma once

/**
 * @file monitor.h
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
#include "socket_p.h"
#include "../buffer/buffer.h"
#include "../utils/threadpool.h"

class Monitor
{
private:
    /* ----- Constructors ----- */

    Monitor(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer);
    Monitor(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer);
    Monitor() = delete;
    void operator=(const Monitor &) = delete;
    ~Monitor();

    /* ----- Members ----- */

    Socket_Listen m_listen2;
    Buffer& m_buffer;
    bool m_stop_operator;

    std::string m_current_kernel_time;
    std::string m_current_real_time;
    std::thread m_time_thread;
    bool m_stop_time_thread;
    std::mutex m_time_mtx;

    static Monitor *m_monitor;
    static Monitor *Get_Instance();
    static Monitor *Get_Instance(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer);
    static Monitor *Get_Instance(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer);

    void operator()();
    void Exit_Loop();

    void UpdateTime();
    void TimeLoop();

    std::queue<std::string> m_log_queue; // 日志数据队列
    std::mutex m_queue_mutex; // 队列互斥锁
    std::condition_variable m_queue_cv; // 队列条件变量
    std::unique_ptr<ThreadPool> m_thread_pool; // 线程池

    void PushLogEntry(const std::string& log_entry); // 推送日志条目到队列
    void ProcessLogEntry(); // 处理日志条目的线程函数

public:
    // Bind operator() and Get_Instance(), return callable object of class Monitor.
    static std::function<void()> Start(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer);
    // Bind operator() and Get_Instance(), return callable object of class Monitor.
    static std::function<void()> Start(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer);

    static void Stop();
};