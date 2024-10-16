#include "monitor.h"

std::unique_ptr<Monitor, std::function<void(Monitor*)>> Monitor::m_monitor = nullptr;

void Monitor::Init()
{
    m_stop_operator = false;
    m_stop_time_thread = false;
    m_log_level = LOG_LEVEL_A - 1;
    m_log_level_symbol[LOG_LEVEL_E] = 'E';
    m_log_level_symbol[LOG_LEVEL_W] = 'W';
    m_log_level_symbol[LOG_LEVEL_D] = 'D';
    m_log_level_symbol[LOG_LEVEL_I] = 'I';
    m_log_level_symbol[LOG_LEVEL_K] = 'K';
    m_log_level_symbol[LOG_LEVEL_L] = 'L';

    /* Thread: Update time */

    UpdateTime();
    m_time_thread = std::thread(&Monitor::TimeLoop, this);

    /* Thread: Log push to buffer */

    m_thread_pool = std::make_unique<ThreadPool>(4);
    for (size_t i = 0; i < 4; ++i)
    {
        m_thread_pool->Enqueue(std::bind(&Monitor::ProcessLogEntry, this));
    }
}

Monitor::Monitor(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer)
    : m_listen2(listen_ip, listen_port)
    , m_buffer(buffer)
{
    this->Init();
}

Monitor::Monitor(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer)
    : m_listen2(listen_ip, listen_port)
    , m_buffer(buffer)
{
    this->Init();
}

Monitor::~Monitor()
{
    // do nothing
}

void Monitor::operator()()
{
    uint8_t command_buffer[100];
    int ret = 0;
    uint64_t log_id = 0;

    while (!m_stop_operator)
    {
        ret = m_listen2.Recv(command_buffer, 100, 100);
        if (ret > 0)
        {
            uint8_t log_level = command_buffer[0];
            std::string log(command_buffer + 1, command_buffer + ret);
            if (log_level & m_log_level)
            {
                std::string log_entry;
                {
                    std::unique_lock<std::mutex> lock(m_time_mtx);
                    log_entry = "[" + m_current_kernel_time + "][" + m_current_real_time + "][" + m_log_level_symbol[log_level] + "]" + log;
                }
                PushLogEntry(std::make_pair(log_id++, log_entry));
            }
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

void Monitor::PushLogEntry(const std::pair<uint64_t, std::string>& log_entry)
{
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_log_queue.push(log_entry);
    }
    m_queue_cv.notify_one();
}

void Monitor::ProcessLogEntry()
{
    while (!m_stop_operator)
    {
        std::pair<uint64_t, std::string> log_entry;
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            m_queue_cv.wait(lock, [this] {
                return !m_log_queue.empty() || m_stop_operator;
            });
            if (m_stop_operator && m_log_queue.empty())
                break;
            log_entry = m_log_queue.front();
            m_log_queue.pop();
        }
        m_buffer.Push(log_entry);
    }
}

std::function<void()> Monitor::Start(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer)
{
    // clang-format off
    if (!m_monitor)
        m_monitor = std::unique_ptr<Monitor, std::function<void(Monitor*)>>
            (new Monitor(listen_ip, listen_port, buffer), [](Monitor* Monitor) { delete Monitor; });
    return std::bind(&Monitor::operator(), &(*m_monitor));
    // clang-format on
}

std::function<void()> Monitor::Start(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer)
{
    // clang-format off
    if (!m_monitor)
        m_monitor = std::unique_ptr<Monitor, std::function<void(Monitor*)>>
            (new Monitor(listen_ip, listen_port, buffer), [](Monitor* Monitor) { delete Monitor; });
    return std::bind(&Monitor::operator(), &(*m_monitor));
    // clang-format on
}

void Monitor::Stop()
{
    m_monitor->m_stop_operator = true;
    m_monitor->m_stop_time_thread = true;
    m_monitor->m_time_thread.join();
    m_monitor->m_queue_cv.notify_all();
}

void Monitor::Set_Log_Level(uint8_t log_level)
{
    m_log_level = log_level;
}

Monitor& Monitor::Get_Instance()
{
    if (!m_monitor)
    {
        throw std::runtime_error("Writer is not initialized");
    }
    return *m_monitor.get();
}