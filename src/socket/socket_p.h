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
 * @ref "/test/case/socket/socket.hpp"
 */
class Socket
{
private:
    // The socket associated with the current program instance.
    struct Socket_Wrap m_local;
    // The socket address of the communication counterpart.
    struct sockaddr_in m_remote;

public:
    Socket() = delete;

    Socket(const char* local_ip, const uint16_t& local_port, 
        const char* target_ip, const uint16_t& target_port);

    Socket(const std::string& local_ip, const uint16_t& local_port, 
        const std::string& target_ip, const uint16_t& target_port);

    ~Socket();

    /**
     * @brief Send data to target.
     * 
     * @param device socket device.
     * @param buffer send buffer.
     * @param n bytes to send.
     * @param target receive from
     * @return int send size.
     */
    int Send(uint8_t* buffer, size_t n);

    /**
     * @brief Receive data from m_listen2.
     * 
     * @param buffer receive buffer
     * @param n bytes to receive.
     * @param timeout_ms if timeout_ms > 0, wait for the time to change and then return; if timeout_ms <= 0, block.
     * 
     * @return success or not.
     * @retval >0, data size.
     * @retval -1, select ret error.
     * @retval -2, timeout.
     */
    int Recv(uint8_t* buffer, size_t n, int timeout_ms);
};

/**
 * @brief A UDP listener class designed to exclusively receive data from the specified m_listen2 address.
 * 
 * @note The archetypal usage scenario involves a singular entity A, characterized by its m_listen2 member variable.
 * @note To illustrate, if the objective is to listen on 127.0.0.1:12345, one need only assign m_listen2 = 127.0.0.1:12345.
 * @note Subsequently, invoking A.Recv() will facilitate the retrieval of incoming data.
 * 
 * @ref "/test/case/socket/socket.hpp"
 */
class Socket_Listen
{
private:
    struct Socket_Wrap m_listen2;

public:
    Socket_Listen() = delete;

    Socket_Listen(const char* listen_ip, const uint16_t& listen_port);

    Socket_Listen(const std::string& listen_ip, const uint16_t& listen_port);

    ~Socket_Listen();

    /**
     * @brief Receive data from m_listen2.
     * 
     * @param buffer receive buffer
     * @param n bytes to receive.
     * @param timeout_ms if timeout_ms > 0, wait for the time to change and then return; if timeout_ms <= 0, block.
     * 
     * @return success or not.
     * @retval >0, data size.
     * @retval -1, select ret error.
     * @retval -2, timeout.
     */
    int Recv(uint8_t* buffer, size_t n, int timeout_ms);
};