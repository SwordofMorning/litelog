#pragma once

/**
 * @file message.h
 * @author master@xiaojintao.email
 * 
 * @brief logger -- message -> buffer
 * which wrap a such log data.
 * 
 * @version 0.1
 * @date 2024-11-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <string>
#include <chrono>
#include <stdint.h>

struct RawMessage
{
    uint8_t level;
    std::string content;
};

class Message
{
public:
    struct TimeInfo
    {
        long kernel_time;
        std::string real_time;
        TimeInfo(long kt, const std::string& rt)
            : kernel_time(kt)
            , real_time(rt)
        {
            // do nothing
        }
    };

private:
    // Index, used to sorting.
    uint64_t m_id;
    // Time struct.
    TimeInfo m_time_info;
    // Log level.
    char m_level;
    // Log String
    std::string m_content;

public:
    Message();
    Message(uint64_t id, const TimeInfo& time_info, char level, const std::string& content);

    uint64_t Get_id() const;
    const TimeInfo& Get_Time_Info() const;
    char Get_Level() const;
    const std::string& Get_Content() const;

    bool operator<(const Message& other) const;

    std::string to_string() const;
};