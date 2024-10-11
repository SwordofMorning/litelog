#include "monitor.h"

Monitor* Monitor::m_monitor= nullptr;

Monitor::Monitor(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer) :
    m_listen2(listen_ip, listen_port), m_buffer(buffer)
{
    m_stop_operator = false;
    m_stop_time_thread = false;
    UpdateTime();
    m_time_thread = std::thread(&Monitor::TimeLoop, this);

}

Monitor::Monitor(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer) :
    m_listen2(listen_ip, listen_port), m_buffer(buffer)
{
    m_stop_operator = false;
    m_stop_time_thread = false;
    UpdateTime();
    m_time_thread = std::thread(&Monitor::TimeLoop, this);
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

    while (!m_stop_operator) 
    {
        ret = m_listen2.Recv(command_buffer, 100, 100);
        if (ret > 0) 
        {
            std::string log(command_buffer, command_buffer + ret);
            std::string log_entry;
            {
                std::unique_lock<std::mutex> lock(m_time_mtx);
                log_entry = "[" + m_current_kernel_time + "][" + m_current_real_time + "] " + log;
            }
            m_buffer.Push(log_entry);
        }
    }
}

void Monitor::UpdateTime() 
{
    // Get kernel time
    struct timespec kernel_time;
    clock_gettime(CLOCK_MONOTONIC, &kernel_time);

    // Calculate kernel uptime in seconds
    long kernel_uptime = kernel_time.tv_sec;

    // Get real time
    struct timespec real_time;
    clock_gettime(CLOCK_REALTIME, &real_time);

    // Format real time
    char real_time_str[64];
    strftime(real_time_str, sizeof(real_time_str), "%Y-%m-%d %H:%M:%S", localtime(&real_time.tv_sec));

    // Update m_current_kernel_time and m_current_real_time
    m_current_kernel_time = std::to_string(kernel_uptime);
    m_current_real_time = real_time_str;
}

void Monitor::TimeLoop() 
{
    while (!m_stop_time_thread) 
    {
        // Shorten the duration of mutex
        {
            std::unique_lock<std::mutex> lock(m_time_mtx);
            UpdateTime();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

std::function<void()> Monitor::Start(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer)
{
    return std::bind(&Monitor::operator(), Monitor::Get_Instance(listen_ip, listen_port, buffer));
}

std::function<void()> Monitor::Start(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer)
{
    return std::bind(&Monitor::operator(), Monitor::Get_Instance(listen_ip, listen_port, buffer));
}

void Monitor::Stop()
{
    m_monitor->m_stop_operator = true;
}