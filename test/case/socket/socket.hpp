#pragma once

#include "../../../src/socket/socket_class.h"
#include "../../utils/utils.h"
#include <thread>
#include <vector>

TEST_CASE(socket_send_recv)
{
    Socket send_socket(Socket{ "127.0.0.1", 10001, "127.0.0.1", 10000 });
    Socket recv_socket(Socket{ "127.0.0.1", 10000, "127.0.0.1", 10001 });

    uint8_t send_buffer[3] = { 0x00, 0x01, 0x02 };
    uint8_t recv_buffer[3];

    send_socket.Send(send_buffer, 3);
    recv_socket.Get(recv_buffer, 3, 10);

    EXPECT_ARR_EQ(send_buffer, recv_buffer, 3);
}