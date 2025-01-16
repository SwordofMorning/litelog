// sink_kernel.cpp
#include "sink_kernel.h"

KernelSink::KernelSink()
    : m_kmsg_fd(-1)
    , m_kernel_msg_pattern(R"(\[(\s*\d+\.\d+)\]\s(.+))")
    , m_dmesg_pipe(nullptr)
    , m_reading_history(true)
{
    // do nothing
}

KernelSink::~KernelSink()
{
    Exit();
}

void KernelSink::Init()
{
    // Open real-time kernel messages
    m_kmsg_fd = open("/dev/kmsg", O_RDONLY | O_NONBLOCK);
    if (m_kmsg_fd < 0)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to open /dev/kmsg");
    }

    // Open dmesg pipe for history logs
    m_dmesg_pipe = popen("dmesg", "r");
    if (!m_dmesg_pipe)
    {
        close(m_kmsg_fd);
        throw std::system_error(errno, std::generic_category(), "Failed to execute dmesg");
    }

    m_processed_messages.clear();
    m_reading_history = true;
}

void KernelSink::Exit()
{
    if (m_kmsg_fd >= 0)
    {
        close(m_kmsg_fd);
        m_kmsg_fd = -1;
    }

    if (m_dmesg_pipe)
    {
        pclose(m_dmesg_pipe);
        m_dmesg_pipe = nullptr;
    }
}

bool KernelSink::ParseKernelMessage(const std::string& raw_line, std::string& timestamp, std::string& message)
{
    std::smatch matches;
    if (std::regex_search(raw_line, matches, m_kernel_msg_pattern))
    {
        timestamp = matches[1].str();
        message = matches[2].str();
        return true;
    }
    return false;
}

std::string KernelSink::ReadDmesgLine()
{
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, m_dmesg_pipe))
    {
        return std::string(buffer);
    }
    return std::string();
}

bool KernelSink::ProcessHistoryLog(RawMessage& raw_msg)
{
    std::string line = ReadDmesgLine();
    if (line.empty())
    {
        m_reading_history = false;
        pclose(m_dmesg_pipe);
        m_dmesg_pipe = nullptr;
        return false;
    }

    std::string timestamp, message;
    if (ParseKernelMessage(line, timestamp, message))
    {
        if (m_processed_messages.find(message) != m_processed_messages.end())
            return false;

        m_processed_messages.insert(message);
        raw_msg.level = LOG_LEVEL_K;
        raw_msg.content = std::move("[" + timestamp + "] " + message);
        return true;
    }
    return false;
}

bool KernelSink::ProcessRealtimeLog(RawMessage& raw_msg)
{
    while (true)
    {
        ssize_t ret = read(m_kmsg_fd, m_recv_buffer, BUFFER_SIZE - 1);

        if (ret < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return false;
            return false;
        }

        if (ret == 0)
            return false;

        m_recv_buffer[ret] = '\0';

        std::string line(m_recv_buffer);
        std::string timestamp, message;

        if (ParseKernelMessage(line, timestamp, message))
        {
            if (m_processed_messages.find(message) != m_processed_messages.end())
                continue;

            m_processed_messages.insert(message);
            raw_msg.level = LOG_LEVEL_K;
            raw_msg.content = std::move("[" + timestamp + "] " + message);
            return true;
        }
    }
    return false;
}

bool KernelSink::Poll(RawMessage& raw_msg)
{
    if (m_reading_history)
    {
        return ProcessHistoryLog(raw_msg);
    }

    return ProcessRealtimeLog(raw_msg);
}