#pragma once

#include <atomic>
#include <map>
#include <functional>
#include "../socket/socket_p.h"
#include "monitor.h"
#include "../buffer/writer.h"

class Controller
{
private:
    Socket m_socket;
    Monitor& m_monitor;
    Writer& m_writer;
    std::atomic<bool> m_stop;

public:
    // clang-format off
    Controller(const char* local_ip, const uint16_t& local_port, 
        const char* target_ip, const uint16_t& target_port,
        Monitor& monitor, Writer& writer);
    Controller(const std::string& local_ip, const uint16_t& local_port, 
        const std::string& target_ip, const uint16_t& target_port,
        Monitor& monitor, Writer& writer);
    // clang-format on

    void operator()();

private:
    using Command = int(Controller::*)(uint8_t *command_offset, uint8_t *result);
    std::map<uint8_t, Command> m_commands;

    int Command_Stop_Program(uint8_t *command_offset, uint8_t *result);
    int Command_Change_Log_Level(uint8_t *command_offset, uint8_t *result);
    int Command_Switch_Page(uint8_t *command_offset, uint8_t *result);
};