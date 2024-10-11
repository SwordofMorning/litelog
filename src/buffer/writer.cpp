#include "writer.h"

Writer::Writer(const std::string& log_path)
{
    /* ----- Step 1: Get Time ----- */

    auto current_time = std::time(nullptr);
    auto local_time = std::localtime(&current_time);

    char time_string[20];
    std::strftime(time_string, sizeof(time_string), "%Y-%m-%d_%H-%M-%S", local_time);

    /* ----- Step 2: File Name ----- */

    std::string log_file_name = log_path + "_" + time_string + ".log";

    /* ----- Step 3: Open File ----- */

    m_log_file.open(log_file_name, std::ios::out);

    /* ----- Step 4: Litelog Info ----- */

    std::strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", local_time);
    Init(time_string);
}

Writer::~Writer()
{
    Exit();
    m_log_file.close();
}

void Writer::Init(const std::string& str_time)
{
    Write("===== Litlog Information =====");

    Write(std::string{"[     Branch     ]: "} + __GIT_BRANCH__);
    Write(std::string{"[      User      ]: "} + __GIT_USER__);
    Write(std::string{"[ Last Commit ID ]: "} + __GIT_COMMIT_ID__);
    Write(std::string{"[  Startup time  ]: "} + str_time);

    Write("===== Litlog Set-up =====");
}

void Writer::Exit()
{
    Write("===== Litlog Tear-down =====");
}

void Writer::Write(const std::string& str)
{
    std::lock_guard<std::mutex> lock(m_file_mutex);
    m_log_file << str << std::endl;
    m_log_file.flush();
}