#pragma once

#include <atomic>
#include <map>
#include <functional>
#include "../utils/socket/socket_p.h"
#include "../logger/logger.h"
#include "../formatter/formatter.h"

class Controller
{
private:
    Socket m_socket;
    Logger& m_monitor;
    Formatter& m_Formatter;
    std::atomic<bool> m_stop;

public:
    // clang-format off
    Controller(const char* local_ip, const uint16_t& local_port, 
        const char* target_ip, const uint16_t& target_port,
        Logger& monitor, Formatter& Formatter);
    Controller(const std::string& local_ip, const uint16_t& local_port, 
        const std::string& target_ip, const uint16_t& target_port,
        Logger& monitor, Formatter& Formatter);
    // clang-format on

    void operator()();

private:
    using Command = int (Controller::*)(uint8_t* command_offset, uint8_t* result);
    std::map<uint8_t, Command> m_commands;

    int Command_Stop_Program(uint8_t* command_offset, uint8_t* result);
    int Command_Change_Log_Level(uint8_t* command_offset, uint8_t* result);
    int Command_Switch_Page(uint8_t* command_offset, uint8_t* result);
};