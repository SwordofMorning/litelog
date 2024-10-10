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

#include "socket_p.h"
#include <string>
#include <map>
#include <functional>
#include <iomanip>

class Monitor
{
private:
    /* ----- Constructors ----- */

    Monitor(const char* listen_ip, const uint16_t& listen_port);
    Monitor(const std::string& listen_ip, const uint16_t& listen_port);
    Monitor() = delete;
    void operator=(const Monitor &) = delete;
    ~Monitor();

    /* ----- Members ----- */

    Socket_Listen m_listen2;
    static Monitor *m_monitor;
    static Monitor *Get_Instance(const char* listen_ip, const uint16_t& listen_port);
    static Monitor *Get_Instance(const std::string& listen_ip, const uint16_t& listen_port);
    void operator()();

public:
    // Bind operator() and Get_Instance(), return callable object of class Monitor.
    static std::function<void()> OBJ(const char* listen_ip, const uint16_t& listen_port);
    // Bind operator() and Get_Instance(), return callable object of class Monitor.
    static std::function<void()> OBJ(const std::string& listen_ip, const uint16_t& listen_port);
};