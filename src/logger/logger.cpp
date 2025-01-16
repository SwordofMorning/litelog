#include "logger.h"

std::unique_ptr<Logger, std::function<void(Logger*)>> Logger::m_logger = nullptr;

void Logger::Init()
{
    m_stop_operator = false;
    m_log_id = 0;

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

    /* Initialize all sinks */
    for (auto& sink : m_sinks)
    {
        sink->Init();
    }

    /* Thread: Log push to buffer */
    m_thread_pool = std::make_unique<ThreadPool>(4);
    for (size_t i = 0; i < 4; ++i)
    {
        m_thread_pool->Enqueue(std::bind(&Logger::ProcessLogEntry, this));
    }
}

Logger::Logger(std::vector<std::unique_ptr<ISink>> sinks, Buffer& buffer)
    : m_sinks(std::move(sinks))
    , m_buffer(buffer)
{
    this->Init();
}

Logger::~Logger()
{
    for (auto& sink : m_sinks)
    {
        if (sink)
        {
            sink->Exit();
        }
    }
}

void Logger::operator()()
{
    RawMessage raw_msg;

    while (!m_stop_operator)
    {
        for (auto& sink : m_sinks)
        {
            if (sink->Poll(raw_msg))
            {
                if (raw_msg.level & m_log_level)
                {
                    // clang-format off
                    Message msg(
                        m_log_id++,
                        Message::TimeInfo(
                            g_kernel_uptime.load(),
                            g_real_time
                        ),
                        m_log_level_symbol[raw_msg.level],
                        raw_msg.content
                    );
                    // clang-format on
                    PushLogEntry(msg);
                }
            }
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
        // clang-format off
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
        // clang-format on
    }
}

std::function<void()> Logger::Start(std::vector<std::unique_ptr<ISink>> sinks, Buffer& buffer)
{
    // clang-format off
    if (!m_logger)
        m_logger = std::unique_ptr<Logger, std::function<void(Logger*)>>
            (new Logger(std::move(sinks), buffer), [](Logger* logger) { delete logger; });
    // clang-format on
    return std::bind(&Logger::operator(), &(*m_logger));
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
        throw std::runtime_error("Logger is not initialized");
    }
    return *m_logger.get();
}