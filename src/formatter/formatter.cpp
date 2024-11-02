#include "formatter.h"

std::unique_ptr<Formatter, std::function<void(Formatter*)>> Formatter::m_formatter = nullptr;

Formatter::Formatter(const std::string& log_path, Buffer& buffer, size_t max_log_lines)
    : m_buffer(buffer)
    , m_stop_write(false)
    , m_max_log_lines(max_log_lines)
    , m_log_path(log_path)
{
    Init();
}

Formatter::~Formatter()
{
    Exit();
}

void Formatter::Init()
{
    /* Step 1: Get Time */

    auto current_time = std::time(nullptr);
    auto local_time = std::localtime(&current_time);

    char time_string[20];
    std::strftime(time_string, sizeof(time_string), "%Y-%m-%d_%H-%M-%S", local_time);

    /* Step 2: File Name */

    std::string log_file_name = m_log_path + "_" + time_string + ".log";

    /* Step 3: Open File */

    m_log_file.open(log_file_name, std::ios::out);

    /* Step 4: Log Info */

    std::strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", local_time);
    Info(time_string);
}

void Formatter::Exit()
{
    m_log_file.close();
}

void Formatter::Info(const std::string& str_time)
{
    Write("===== litlog Information =====");

    Write(std::string{"[     Branch     ]: "} + __GIT_BRANCH__);
    Write(std::string{"[      User      ]: "} + __GIT_USER__);
    Write(std::string{"[ Last Commit ID ]: "} + __GIT_COMMIT_ID__);
    Write(std::string{"[  Startup time  ]: "} + str_time);

    Write("===== litlog Set-up =====");
}

void Formatter::Write(const Message& msg)
{
    std::lock_guard<std::mutex> lock(m_file_mutex);

    m_log_file << msg.to_string() << std::endl;

    m_log_file.flush();
    ++m_lines_written;
}

void Formatter::Write(const std::string& str)
{
    std::lock_guard<std::mutex> lock(m_file_mutex);
    m_log_file << str << std::endl;
    m_log_file.flush();
    ++m_lines_written;
}

void Formatter::operator()()
{
    while (!m_stop_write)
    {
        // Shorten the duration of mutex
        {
            std::unique_lock<std::mutex> lock(m_write_mutex);
            if (m_lines_written >= m_max_log_lines)
                Switch();
        }

        {
            std::unique_lock<std::mutex> lock(m_write_mutex);
            while (!m_buffer.IsL2Empty())
            {
                Message msg = m_buffer.Pull(std::chrono::milliseconds(10));
                if (msg.Get_Content().empty())
                    continue;
                Write(msg);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

std::function<void()> Formatter::Start(const std::string& log_path, Buffer& buffer, size_t max_log_lines)
{
    // clang-format off
    if (!m_formatter)
        m_formatter = std::unique_ptr<Formatter, std::function<void(Formatter*)>>
            (new Formatter(log_path, buffer, max_log_lines), [](Formatter* Formatter) { delete Formatter; });
    return std::bind(&Formatter::operator(), &(*m_formatter));
    // clang-format on
}

void Formatter::Stop()
{
    m_formatter->m_stop_write = true;
    m_formatter->Write("===== litlog Tear-down =====");
}

void Formatter::Switch()
{
    Write("===== litlog Switch-page =====");
    Exit();
    m_lines_written = 0;
    Init();
}

Formatter& Formatter::Get_Instance()
{
    if (!m_formatter)
    {
        throw std::runtime_error("Formatter is not initialized");
    }
    return *m_formatter.get();
}