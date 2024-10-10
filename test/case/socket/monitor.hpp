#pragma once

#include "../../../src/socket/monitor.h"
#include "../../utils/utils.h"
#include <thread>
#include <vector>

TEST_CASE(monitor)
{
    std::thread monitor{ Monitor::OBJ("127.0.0.1", 10000) };
    monitor.detach();

    Socket send_socket(Socket{ "127.0.0.1", 10001, "127.0.0.1", 10000 });
    uint8_t send_buffer[3] = { 0x00, 0x01, 0x02 };
    uint8_t recv_buffer[3];

    sleep(1);

    send_socket.Send(send_buffer, 3);
}