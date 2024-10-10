#include "monitor.h"

Monitor* Monitor::m_monitor= nullptr;

Monitor::Monitor(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer) :
    m_listen2(listen_ip, listen_port), m_buffer(buffer)
{
    // do nothing
}

Monitor::Monitor(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer) :
    m_listen2(listen_ip, listen_port), m_buffer(buffer)
{
    // do nothing
}

Monitor::~Monitor()
{
    if (m_monitor) delete m_monitor;
}

Monitor *Monitor::Get_Instance(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer)
{
    return m_monitor == nullptr ? 
        (m_monitor = new Monitor(listen_ip, listen_port, buffer)) : 
        m_monitor;
}

Monitor *Monitor::Get_Instance(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer)
{
    return m_monitor == nullptr ? 
        (m_monitor = new Monitor(listen_ip, listen_port, buffer)) : 
        m_monitor;
}

void Monitor::operator()()
{
    uint8_t command_buffer[100];
    int ret = 0;

    while (true) 
    {
        ret = m_listen2.Recv(command_buffer, 100, 0);
        if (ret > 0) 
        {
            std::string log(command_buffer, command_buffer + ret);
            m_buffer.PutToL1(log);
        }
    }
}

std::function<void()> Monitor::OBJ(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer)
{
    return std::bind(&Monitor::operator(), Monitor::Get_Instance(listen_ip, listen_port, buffer));
}

std::function<void()> Monitor::OBJ(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer)
{
    return std::bind(&Monitor::operator(), Monitor::Get_Instance(listen_ip, listen_port, buffer));
}