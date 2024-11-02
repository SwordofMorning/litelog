#include "logger.h"

std::unique_ptr<Logger, std::function<void(Logger*)>> Logger::m_logger = nullptr;

void Logger::Init()
{
    m_stop_operator = false;

    /* Log level */

    m_log_level = LOG_LEVEL_A;
    m_log_level_symbol[LOG_LEVEL_F] = 'F';
    m_log_level_symbol[LOG_LEVEL_E] = 'E';
    m_log_level_symbol[LOG_LEVEL_W] = 'W';
    m_log_level_symbol[LOG_LEVEL_N] = 'N';
    m_log_level_symbol[LOG_LEVEL_I] = 'I';
    m_log_level_symbol[LOG_LEVEL_D] = 'D';
    m_log_level_symbol[LOG_LEVEL_T] = 'T';
    m_log_level_symbol[LOG_LEVEL_K] = 'K';

    /* Thread: Log push to buffer */

    m_thread_pool = std::make_unique<ThreadPool>(4);
    for (size_t i = 0; i < 4; ++i)
    {
        m_thread_pool->Enqueue(std::bind(&Logger::ProcessLogEntry, this));
    }
}

Logger::Logger(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer)
    : m_listen2(listen_ip, listen_port)
    , m_buffer(buffer)
{
    this->Init();
}

Logger::Logger(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer)
    : m_listen2(listen_ip, listen_port)
    , m_buffer(buffer)
{
    this->Init();
}

Logger::~Logger()
{
    // do nothing
}

void Logger::operator()()
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
            std::string content(command_buffer + 1, command_buffer + ret);

            // clang-format off
            if (log_level & m_log_level)
            {
                Message msg(
                    log_id++,                         // 消息ID
                    Message::TimeInfo(                // 时间信息
                        g_kernel_uptime.load(),       // 内核时间
                        g_real_time                   // 真实时间
                    ),
                    m_log_level_symbol[log_level],    // 日志级别
                    content                           // 日志内容
                );
                
                PushLogEntry(msg);
            }
            // clang-format on
        }
    }
}

void Logger::PushLogEntry(const Message& msg)
{
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_log_queue.push(msg);
    }
    m_queue_cv.notify_one();
}

void Logger::ProcessLogEntry()
{
    while (!m_stop_operator)
    {
        Message msg;
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            m_queue_cv.wait(lock, [this] {
                return !m_log_queue.empty() || m_stop_operator;
            });
            if (m_stop_operator && m_log_queue.empty())
                break;

            msg = m_log_queue.front();
            m_log_queue.pop();
        }
        m_buffer.Push(msg);
    }
}

std::function<void()> Logger::Start(const char* listen_ip, const uint16_t& listen_port, Buffer& buffer)
{
    // clang-format off
    if (!m_logger)
        m_logger = std::unique_ptr<Logger, std::function<void(Logger*)>>
            (new Logger(listen_ip, listen_port, buffer), [](Logger* Logger) { delete Logger; });
    return std::bind(&Logger::operator(), &(*m_logger));
    // clang-format on
}

std::function<void()> Logger::Start(const std::string& listen_ip, const uint16_t& listen_port, Buffer& buffer)
{
    // clang-format off
    if (!m_logger)
        m_logger = std::unique_ptr<Logger, std::function<void(Logger*)>>
            (new Logger(listen_ip, listen_port, buffer), [](Logger* Logger) { delete Logger; });
    return std::bind(&Logger::operator(), &(*m_logger));
    // clang-format on
}

void Logger::Stop()
{
    m_logger->m_stop_operator = true;
    m_logger->m_queue_cv.notify_all();
}

void Logger::Set_Log_Level(uint8_t log_level)
{
    m_log_level = log_level;
}

Logger& Logger::Get_Instance()
{
    if (!m_logger)
    {
        throw std::runtime_error("Writer is not initialized");
    }
    return *m_logger.get();
}