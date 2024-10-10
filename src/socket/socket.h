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

#include "../config/config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Init socket device.
 * 
 * @param p_socket which socket will be initted.
 * @param p_self_ip socket's ip.
 * @param p_self_port socket's port.
 * @return int 
 */
int Socket_Init(struct Socket_Wrap *p_socket, const char* p_self_ip, uint16_t p_self_port);

/**
 * @brief Create Socket link target.
 * 
 * @param p_target sockaddr_in struct, only used to wrap ip and port.
 * @param p_target_ip target ip.
 * @param p_target_port target port.
 * 
 * @note p_target is JUST a wrapper, which warp ip address and port of remote(target).
 */
void Socket_Create_Target(struct sockaddr_in* p_target, const char* p_target_ip, uint16_t p_target_port);

/**
 * @brief Send data to target.
 * 
 * @param device socket device.
 * @param buffer send buffer.
 * @param n bytes to send.
 * @param target receive from
 * @return int send size.
 */
int Socket_Send(int device, uint8_t* buffer, size_t n, struct sockaddr* target);

/**
 * @brief Get data from target.
 * 
 * @param device socket device.
 * @param buffer receive buffer
 * @param n bytes to receive.
 * @param target receive from.
 * @param timeout_ms timeout parameter.
 * @return int receive size.
 */
int Socket_Get(int device, uint8_t* buffer, size_t n, struct sockaddr* target, int timeout_ms);

#ifdef __cplusplus
}
#endif