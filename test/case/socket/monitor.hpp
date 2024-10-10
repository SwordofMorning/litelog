#pragma once

#include "../../../src/socket/monitor.h"
#include "../../../src/buffer/buffer.h"
#include "../../utils/utils.h"
#include <thread>
#include <vector>
#include <iostream>

TEST_CASE(monitor)
{
    Buffer buf(200, 200);

    std::thread monitor{ Monitor::Start("127.0.0.1", 10000, buf) };

    Socket send_socket(Socket{ "127.0.0.1", 10001, "127.0.0.1", 10000 });
    uint8_t send_buffer_1[3] = { 'A', 'B', 'C' };
    uint8_t send_buffer_2[3] = { 'E', 'F', 'G' };
    uint8_t recv_buffer[3];
    std::string exp_1 = "ABC";
    std::string exp_2 = "EFG";

    send_socket.Send(send_buffer_1, 3);
    send_socket.Send(send_buffer_2, 3);

    sleep(1);

    EXPECT_STR_EQ(exp_1.c_str(), buf.Pull(std::chrono::milliseconds(10)).c_str());
    EXPECT_STR_EQ(exp_2.c_str(), buf.Pull(std::chrono::milliseconds(10)).c_str());

    send_socket.Send(send_buffer_1, 3);
    send_socket.Send(send_buffer_2, 3);
    send_socket.Send(send_buffer_1, 3);

    sleep(1);

    EXPECT_STR_EQ(exp_1.c_str(), buf.Pull(std::chrono::milliseconds(10)).c_str());
    EXPECT_STR_EQ(exp_2.c_str(), buf.Pull(std::chrono::milliseconds(10)).c_str());
    EXPECT_STR_EQ(exp_1.c_str(), buf.Pull(std::chrono::milliseconds(10)).c_str());

    Monitor::Stop();
    monitor.join();
}