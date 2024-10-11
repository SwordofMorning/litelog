#pragma once

#include "../../../src/buffer/writer.h"
#include "../../../src/buffer/buffer.h"
#include "../../../src/socket/monitor.h"
#include "../../utils/utils.h"
#include <cassert>
#include <cstdint>
#include <iostream>


TEST_CASE(writer_write_from_buffer)
{
    Writer write("/root/Unit");

    Socket send_socket(Socket{ "127.0.0.1", 10001, "127.0.0.1", 12345 });
    uint8_t send_buffer_1[3] = { 'I', 'B', 'C' };
    uint8_t send_buffer_2[3] = { 'I', 'F', 'G' };

    std::string send_info{"I[program1] Hello World!"};
    std::string send_debug{"D[program1] Here is debug information."};
    std::string send_warning{"W[program1] Warning!"};
    std::string send_error{"E[program1] Error!"};
    std::string send_kernel{"K[kernel] Kernel information"};

    // 发送 uint8_t 数组
    send_socket.Send(send_buffer_1, 3);
    send_socket.Send(send_buffer_2, 3);

    // 将 std::string 转换为 uint8_t 数组并发送
    send_socket.Send(reinterpret_cast<const uint8_t*>(send_info.c_str()), send_info.size());
    send_socket.Send(reinterpret_cast<const uint8_t*>(send_debug.c_str()), send_debug.size());
    send_socket.Send(reinterpret_cast<const uint8_t*>(send_warning.c_str()), send_warning.size());
    send_socket.Send(reinterpret_cast<const uint8_t*>(send_error.c_str()), send_error.size());
    send_socket.Send(reinterpret_cast<const uint8_t*>(send_kernel.c_str()), send_kernel.size());

    sleep(1);

    write.Write(buf.Pull(std::chrono::milliseconds(10)));
    write.Write(buf.Pull(std::chrono::milliseconds(10)));
    write.Write(buf.Pull(std::chrono::milliseconds(10)));
    write.Write(buf.Pull(std::chrono::milliseconds(10)));
    write.Write(buf.Pull(std::chrono::milliseconds(10)));
    write.Write(buf.Pull(std::chrono::milliseconds(10)));
    write.Write(buf.Pull(std::chrono::milliseconds(10)));
}