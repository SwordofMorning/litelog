#include "clock.h"

std::unique_ptr<Clock, std::function<void(Clock*)>> Clock::m_instance = nullptr;

Clock::Clock()
    : m_stop_flag(false)
{
    m_update_thread = std::thread(&Clock::UpdateLoop, this);
}

Clock::~Clock()
{
    if (m_update_thread.joinable())
    {
        m_stop_flag = true;
        m_update_thread.join();
    }
}

void Clock::UpdateTime()
{
    // Get kernel time
    struct timespec kernel_time;
    clock_gettime(CLOCK_MONOTONIC, &kernel_time);
    g_kernel_uptime.store(kernel_time.tv_sec);

    // Get real time
    struct timespec real_time;
    clock_gettime(CLOCK_REALTIME, &real_time);

    // Format real time
    std::unique_lock<std::mutex> lock(g_time_mutex);
    strftime(g_real_time, sizeof(g_real_time), "%Y-%m-%d %H:%M:%S", localtime(&real_time.tv_sec));
}

void Clock::UpdateLoop()
{
    while (!m_stop_flag)
    {
        UpdateTime();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void Clock::Start()
{
    if (!m_instance)
    {
        m_instance = std::unique_ptr<Clock, std::function<void(Clock*)>>(new Clock(), [](Clock* clock) {
            delete clock;
        });
    }
}

void Clock::Stop()
{
    if (m_instance)
    {
        m_instance->m_stop_flag = true;
    }
}

Clock& Clock::Get_Instance()
{
    if (!m_instance)
    {
        throw std::runtime_error("Clock is not initialized");
    }
    return *m_instance;
}