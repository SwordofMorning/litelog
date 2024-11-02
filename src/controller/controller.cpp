#include "controller.h"

Controller::Controller(const char* local_ip, const uint16_t& local_port, const char* target_ip, const uint16_t& target_port, Logger& monitor, Writer& writer)
    : m_stop(false)
    , m_socket(local_ip, local_port, target_ip, target_port)
    , m_monitor(monitor)
    , m_writer(writer)
{
    // Function Map
    this->m_commands[CTL_STOP_PROGRAM] = &Controller::Command_Stop_Program;
    this->m_commands[CTL_CHANGE_LEVEL] = &Controller::Command_Change_Log_Level;
    this->m_commands[CTL_SWITCH_PAGE] = &Controller::Command_Switch_Page;
}

Controller::Controller(const std::string& local_ip, const uint16_t& local_port, const std::string& target_ip, const uint16_t& target_port, Logger& monitor, Writer& writer)
    : m_stop(false)
    , m_socket(local_ip.c_str(), local_port, target_ip.c_str(), target_port)
    , m_monitor(monitor)
    , m_writer(writer)
{
    // Function Map
    this->m_commands[CTL_STOP_PROGRAM] = &Controller::Command_Stop_Program;
    this->m_commands[CTL_CHANGE_LEVEL] = &Controller::Command_Change_Log_Level;
    this->m_commands[CTL_SWITCH_PAGE] = &Controller::Command_Switch_Page;
}

void Controller::operator()()
{
    const size_t n = 10;
    uint8_t command[n];
    uint8_t result[n];

    while (!m_stop)
    {
        // pre-declare for goto.
        int ret = 0;
        CLEAR(command);
        CLEAR(result);
        result[0] = 0x00;

        // Get socket data.
        ret = m_socket.Recv(command, n, 0);

        if (ret < 0)
            continue;

        // Command exited?
        if (m_commands.find(command[0]) == m_commands.end())
        {
            result[0] = 0x01;
            goto return_result;
        }

        // Call function
        ret = (this->*this->m_commands[command[0]])(command + 1, result);
        // Execute fail
        if (ret != 0)
        {
            result[0] = 0x02;
            goto return_result;
        }

    return_result:
        ret = m_socket.Send(result, n);
    }
}

int Controller::Command_Stop_Program(uint8_t* command_offset, uint8_t* result)
{
    m_stop = true;
    return 0;
}

int Controller::Command_Change_Log_Level(uint8_t* command_offset, uint8_t* result)
{
    m_monitor.Set_Log_Level(command_offset[0]);
    return 0;
}

int Controller::Command_Switch_Page(uint8_t* command_offset, uint8_t* result)
{
    m_writer.Switch();
    return 0;
}