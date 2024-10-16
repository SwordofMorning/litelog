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

#ifdef __cplusplus
extern "C" {
#endif

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
struct sockaddr_in server;

void Litelog_Init(char* local_ip, uint16_t local_port, char* target_ip, uint16_t target_port)
{
    // Init local socket.
    Socket_Init(&local, local_ip, local_port);
    // Wrap remote ip and port.
    Socket_Create_Target(&server, target_ip, target_port);
}

int Litelog_Send(uint8_t* buffer, size_t n)
{
    return Socket_Send(local.device, buffer, n, (struct sockaddr*)&server);
}

void Litlog_Exit()
{
    Socket_Exit(&local);
}

#ifdef __cplusplus
}
#endif