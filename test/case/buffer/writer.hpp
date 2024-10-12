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
    uint8_t send_buffer_1[3] = { 0x04, 'B', 'C' };
    uint8_t send_buffer_2[3] = { 0x08, 'F', 'G' };

    // [0] = 1
    std::string send_error{"[program1] Error!"};
    // [0] = 2
    std::string send_warning{"[program1] Warning!"};
    // [0] = 4
    std::string send_debug{"[program1] Here is debug information."};
    // [0] = 8
    std::string send_info{"[program1] Hello World!"};
    // [0] = 16
    std::string send_kernel{"[kernel] Kernel information"};

    // Sent uint8_t
    send_socket.Send(send_buffer_1, 3);
    send_socket.Send(send_buffer_2, 3);

    // Send string with cast

    std::vector<uint8_t> info_buffer(send_info.size() + 1);
    info_buffer[0] = 0x08;
    std::copy(send_info.begin(), send_info.end(), info_buffer.begin() + 1);
    send_socket.Send(info_buffer.data(), info_buffer.size());

    std::vector<uint8_t> debug_buffer(send_debug.size() + 1);
    debug_buffer[0] = 0x04;
    std::copy(send_debug.begin(), send_debug.end(), debug_buffer.begin() + 1);
    send_socket.Send(debug_buffer.data(), debug_buffer.size());

    std::vector<uint8_t> warning_buffer(send_warning.size() + 1);
    warning_buffer[0] = 0x02;
    std::copy(send_warning.begin(), send_warning.end(), warning_buffer.begin() + 1);
    send_socket.Send(warning_buffer.data(), warning_buffer.size());

    std::vector<uint8_t> error_buffer(send_error.size() + 1);
    error_buffer[0] = 0x01;
    std::copy(send_error.begin(), send_error.end(), error_buffer.begin() + 1);
    send_socket.Send(error_buffer.data(), error_buffer.size());

    std::vector<uint8_t> kernel_buffer(send_kernel.size() + 1);
    kernel_buffer[0] = 0x10;
    std::copy(send_kernel.begin(), send_kernel.end(), kernel_buffer.begin() + 1);
    send_socket.Send(kernel_buffer.data(), kernel_buffer.size());

    sleep(1);

    while (!buf.IsL2Empty()) write.Write(buf.Pull(std::chrono::milliseconds(10)));

    // Change log level
    uint8_t send_change_1[2] = { LOG_CTL_LEVEL_CHANGE, LOG_LEVEL_I };
    send_socket.Send(send_change_1, 2);

    send_socket.Send(info_buffer.data(), info_buffer.size());
    send_socket.Send(debug_buffer.data(), debug_buffer.size());
    send_socket.Send(warning_buffer.data(), warning_buffer.size());
    send_socket.Send(error_buffer.data(), error_buffer.size());
    send_socket.Send(kernel_buffer.data(), kernel_buffer.size());

    sleep(1);

    while (!buf.IsL2Empty()) write.Write(buf.Pull(std::chrono::milliseconds(10)));

    // Change log level
    uint8_t send_change_2[2] = { LOG_CTL_LEVEL_CHANGE, LOG_LEVEL_K | LOG_LEVEL_D };
    send_socket.Send(send_change_2, 2);

    send_socket.Send(info_buffer.data(), info_buffer.size());
    send_socket.Send(debug_buffer.data(), debug_buffer.size());
    send_socket.Send(warning_buffer.data(), warning_buffer.size());
    send_socket.Send(error_buffer.data(), error_buffer.size());
    send_socket.Send(kernel_buffer.data(), kernel_buffer.size());

    sleep(1);

    while (!buf.IsL2Empty()) write.Write(buf.Pull(std::chrono::milliseconds(10)));

    // uint8_t send_exit[1] = { LOG_CTL_EXIT };
    // send_socket.Send(send_exit, 1);
}