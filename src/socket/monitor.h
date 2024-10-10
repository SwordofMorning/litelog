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
#include "socket_p.h"
#include "../buffer/buffer.h"

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
    bool m_stop;

    static Monitor *m_monitor;
    static Monitor *Get_Instance();
    static Monitor *Get_Instance(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer);
    static Monitor *Get_Instance(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer);

    void operator()();
    void Exit_Loop();

public:
    // Bind operator() and Get_Instance(), return callable object of class Monitor.
    static std::function<void()> Start(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer);
    // Bind operator() and Get_Instance(), return callable object of class Monitor.
    static std::function<void()> Start(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer);

    static void Stop();
};