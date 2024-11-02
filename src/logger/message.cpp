#include "message.h"

Message::Message()
    : m_id(0)
    , m_time_info(0, "")
    , m_level('\0')
    , m_content("")
{
    // do nothing
}

Message::Message(uint64_t id, const TimeInfo& time_info, char level, const std::string& content)
    : m_id(id)
    , m_time_info(time_info)
    , m_level(level)
    , m_content(content)
{
    // do nothing
}

uint64_t Message::Get_id() const
{
    return m_id;
}

const Message::TimeInfo& Message::Get_Time_Info() const
{
    return m_time_info;
}

char Message::Get_Level() const
{
    return m_level;
}

const std::string& Message::Get_Content() const
{
    return m_content;
}

bool Message::operator<(const Message& other) const
{
    return m_id < other.m_id;
}

std::string Message::to_string() const
{
    // clang-format off
    return "[" + std::to_string(m_time_info.kernel_time) + "]"
         + "[" + m_time_info.real_time + "]"
         + "[" + m_level + "]"
         + m_content;
    // clang-format on
}