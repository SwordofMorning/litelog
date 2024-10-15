#include "writer.h"

std::unique_ptr<Writer, std::function<void(Writer*)>> Writer::m_writer = nullptr;

Writer::Writer(const std::string& log_path, Buffer& buffer, size_t max_log_lines)
    : m_buffer(buffer)
    , m_stop_write(false)
    , m_max_log_lines(max_log_lines)
    , m_log_path(log_path)
{
    Init();
}

Writer::~Writer()
{
    Exit();
}

void Writer::Init()
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

void Writer::Exit()
{
    m_log_file.close();
}

void Writer::Info(const std::string& str_time)
{
    Write("===== litlog Information =====");

    Write(std::string{"[     Branch     ]: "} + __GIT_BRANCH__);
    Write(std::string{"[      User      ]: "} + __GIT_USER__);
    Write(std::string{"[ Last Commit ID ]: "} + __GIT_COMMIT_ID__);
    Write(std::string{"[  Startup time  ]: "} + str_time);

    Write("===== litlog Set-up =====");
}

void Writer::Write(const std::string& str)
{
    std::lock_guard<std::mutex> lock(m_file_mutex);
    m_log_file << str << std::endl;
    m_log_file.flush();
    ++m_lines_written;
}

void Writer::operator()()
{
    while (!m_stop_write)
    {
        // Shorten the duration of mutex
        {
            std::unique_lock<std::mutex> lock(m_write_mutex);
            if (m_lines_written >= m_max_log_lines)
                Switch();
        }

        /**
         * @todo Paging is not strictly based on m_max_log_lines.
         */

        {
            std::unique_lock<std::mutex> lock(m_write_mutex);
            while (!m_buffer.IsL2Empty())
                Write(m_buffer.Pull(std::chrono::milliseconds(10)));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

std::function<void()> Writer::Start(const std::string& log_path, Buffer& buffer, size_t max_log_lines)
{
    // clang-format off
    if (!m_writer)
        m_writer = std::unique_ptr<Writer, std::function<void(Writer*)>>
            (new Writer(log_path, buffer, max_log_lines), [](Writer* Writer) { delete Writer; });
    return std::bind(&Writer::operator(), &(*m_writer));
    // clang-format on
}

void Writer::Stop()
{
    m_writer->m_stop_write = true;
    m_writer->Write("===== litlog Tear-down =====");
}

void Writer::Switch()
{
    Write("===== litlog Switch-page =====");
    Exit();
    m_lines_written = 0;
    Init();
}

Writer& Writer::Get_Instance()
{
    if (!m_writer)
    {
        throw std::runtime_error("Writer is not initialized");
    }
    return *m_writer.get();
}