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

    std::thread monitor{ Monitor::OBJ("127.0.0.1", 10000, buf) };
    monitor.detach();

    Socket send_socket(Socket{ "127.0.0.1", 10001, "127.0.0.1", 10000 });
    uint8_t send_buffer[3] = { 'A', 'B', 'C' };
    uint8_t recv_buffer[3];

    sleep(1);

    send_socket.Send(send_buffer, 3);

    sleep(1);

    buf.CopyFromL1ToL2();
    std::cout << buf.GetFromL2() << std::endl;
}