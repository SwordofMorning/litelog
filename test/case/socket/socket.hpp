#pragma once

#include "../../../src/socket/socket_p.h"
#include "../../utils/utils.h"
#include <thread>
#include <vector>

TEST_CASE(socket_unidirectional)
{
    Socket recv_socket(Socket{ "127.0.0.1", 10000, "127.0.0.1", 10001 });
    Socket send_socket(Socket{ "127.0.0.1", 10001, "127.0.0.1", 10000 });

    uint8_t send_buffer[3] = { 0x00, 0x01, 0x02 };
    uint8_t recv_buffer[3];

    send_socket.Send(send_buffer, 3);
    recv_socket.Recv(recv_buffer, 3, 10);

    EXPECT_ARR_EQ(send_buffer, recv_buffer, 3);
}

TEST_CASE(socket_bidirectional)
{
    Socket socket_A(Socket{ "127.0.0.1", 10000, "127.0.0.1", 10001 });
    Socket socket_B(Socket{ "127.0.0.1", 10001, "127.0.0.1", 10000 });

    /* Firstly, A send data to B */

    uint8_t send_buffer_A2B[3] = { 0x00, 0x01, 0x02 };
    uint8_t recv_buffer_A2B[3];

    socket_A.Send(send_buffer_A2B, 3);
    socket_B.Recv(recv_buffer_A2B, 3, 10);

    EXPECT_ARR_EQ(send_buffer_A2B, recv_buffer_A2B, 3);

    /* Then, B send data to A */

    uint8_t send_buffer_B2A[3] = { 0x03, 0x04, 0x05 };
    uint8_t recv_buffer_B2A[3];

    socket_B.Send(send_buffer_B2A, 3);
    socket_A.Recv(recv_buffer_B2A, 3, 10);

    EXPECT_ARR_EQ(send_buffer_B2A, recv_buffer_B2A, 3);
}

TEST_CASE(socket_listen)
{
    Socket_Listen recv_socket("127.0.0.1", 12345);
    Socket send_socket("127.0.0.1", 54321, "127.0.0.1", 12345);

    uint8_t send_buffer[3] = { 0x00, 0x01, 0x02 };
    uint8_t recv_buffer[3];

    send_socket.Send(send_buffer, 3);
    recv_socket.Recv(recv_buffer, 3, 10);

    EXPECT_ARR_EQ(send_buffer, recv_buffer, 3);
}