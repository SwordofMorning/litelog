#pragma once

#include "../../../src/socket/monitor.h"
#include "../../../src/buffer/buffer.h"
#include "../../utils/utils.h"
#include <thread>
#include <vector>
#include <iostream>

TEST_CASE(monitor_and_buf)
{
    Socket send_socket(Socket{ "127.0.0.1", 10001, "127.0.0.1", 12345 });
    uint8_t send_buffer_1[3] = { 'I', 'B', 'C' };
    uint8_t send_buffer_2[3] = { 'I', 'F', 'G' };
    uint8_t recv_buffer[3];
    std::string exp_1 = "BC";
    std::string exp_2 = "FG";

    send_socket.Send(send_buffer_1, 3);
    send_socket.Send(send_buffer_2, 3);

    sleep(1);

    std::string str(buf.Pull(std::chrono::milliseconds(10)));
    EXPECT_STR_EQ(exp_1.c_str(), str.substr(str.size() - 2).c_str());
    str = buf.Pull(std::chrono::milliseconds(10));
    EXPECT_STR_EQ(exp_2.c_str(), str.substr(str.size() - 2).c_str());

    send_socket.Send(send_buffer_1, 3);
    send_socket.Send(send_buffer_2, 3);
    send_socket.Send(send_buffer_1, 3);

    sleep(1);

    str = buf.Pull(std::chrono::milliseconds(10));
    EXPECT_STR_EQ(exp_1.c_str(), str.substr(str.size() - 2).c_str());
    str = buf.Pull(std::chrono::milliseconds(10));
    EXPECT_STR_EQ(exp_2.c_str(), str.substr(str.size() - 2).c_str());
    str = buf.Pull(std::chrono::milliseconds(10));
    EXPECT_STR_EQ(exp_1.c_str(), str.substr(str.size() - 2).c_str());
}