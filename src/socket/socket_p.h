#pragma once

/**
 * @file socket_p.h
 * @author master@xiaojintao.email
 * @brief Socket cpp class interface
 * @version 0.1
 * @date 2024-10-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "../config/config.h"
#include "socket_c.h"
#include <string>
#include <map>
#include <functional>
#include <iomanip>
#include <chrono>

/**
 * @brief A Point-to-Point UDP Socket class facilitating seamless data transmission between two processes or threads.
 * 
 * @note The quintessential usage of this class involves two entities, A and B, engaging in bidirectional communication.
 * @note For entity A, set m_local to 127.0.0.1:10000; for entity B, set m_local to 127.0.0.1:10001.
 * @note Subsequently, assign A:m_remote = B:m_local and B:m_local = A:m_remote to establish the connection.
 * @note With this configuration, invoking A.Send() will transmit data to B, which can be retrieved using B.Get().
 * @note This elegant design allows for efficient and reliable data exchange between the two endpoints.
 * 
 * @ref You can see the typical usage in "/test/case/socket.hpp", socket_p2p_unidirectional and socket_p2p_bidirectional.
 */
class Socket_P2P
{
private:
    // The socket associated with the current program instance.
    struct Socket_Wrap m_local;
    // The socket address of the communication counterpart.
    struct sockaddr_in m_remote;

public:
    Socket_P2P() = delete;

    Socket_P2P(const char* local_ip, const uint16_t& local_port, 
        const char* target_ip, const uint16_t& target_port);

    Socket_P2P(const std::string& local_ip, const uint16_t& local_port, 
        const std::string& target_ip, const uint16_t& target_port);

    ~Socket_P2P();

    int Send(uint8_t* buffer, size_t n);

    int Get(uint8_t* buffer, size_t n, int timeout_ms);
};

