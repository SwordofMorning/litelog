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
}

Writer::~Writer()
{
    m_log_file.close();
}

void Writer::Write(const std::string& str)
{
    std::lock_guard<std::mutex> lock(m_file_mutex);
    m_log_file << str << std::endl;
    m_log_file.flush();
}