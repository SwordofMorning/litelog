#pragma once

#include "../../../src/buffer/writer.h"
#include "../../../src/buffer/buffer.h"
#include "../../../src/socket/monitor.h"
#include "../../utils/utils.h"

TEST_CASE(writer_write_from_buffer)
{
    Writer write("/root/Unit");

    Socket send_socket(Socket{ "127.0.0.1", 10001, "127.0.0.1", 12345 });
    uint8_t send_buffer_1[3] = { 'A', 'B', 'C' };
    uint8_t send_buffer_2[3] = { 'E', 'F', 'G' };

    send_socket.Send(send_buffer_1, 3);
    send_socket.Send(send_buffer_2, 3);

    sleep(1);

    std::string str = buf.Pull(std::chrono::milliseconds(10));
    write.Write(str);
}