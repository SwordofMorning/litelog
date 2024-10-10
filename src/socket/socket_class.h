#pragma once

#include "../config/config.h"
#include "socket.h"
#include <string>
#include <map>
#include <functional>
#include <iomanip>
#include <chrono>

class Socket
{
private:
    // Socket for this program.
    struct Socket_Wrap m_local;
    // Address for communication target.
    struct sockaddr_in m_remote;

public:
    Socket() = delete;

    Socket(const char* local_ip, const uint16_t& local_port, 
        const char* target_ip, const uint16_t& target_port)
    {
        // Init local socket.
        Socket_Init(&m_local, local_ip, local_port);
        // Wrap remote ip and port.
        Socket_Create_Target(&m_remote, target_ip, target_port);
    }

    Socket(const std::string& local_ip, const uint16_t& local_port, 
        const std::string& target_ip, const uint16_t& target_port)
    {
        // Init local socket.
        Socket_Init(&m_local, local_ip.c_str(), local_port);
        // Wrap remote ip and port.
        Socket_Create_Target(&m_remote, target_ip.c_str(), target_port);
    }

    int Send(uint8_t* buffer, size_t n)
    {
        return Socket_Send(m_local.device, buffer, n, (sockaddr*)&m_remote);
    }

    int Get(uint8_t* buffer, size_t n, int timeout_ms)
    {
        return Socket_Get(m_local.device, buffer, n, (sockaddr*)&m_remote, timeout_ms);
    }
};