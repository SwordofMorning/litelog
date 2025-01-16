// sink_kernel.h
#pragma once

#include <set>
#include <string>
#include <regex>
#include <fcntl.h>
#include <unistd.h>
#include <system_error>
#include "sink.h"

class KernelSink : public ISink
{
private:
    static constexpr size_t BUFFER_SIZE = 1024;
    char m_recv_buffer[BUFFER_SIZE];
    int m_kmsg_fd;

    std::set<std::string> m_processed_messages;
    std::regex m_kernel_msg_pattern;

    // Add new members for dmesg processing
    FILE* m_dmesg_pipe;
    bool m_reading_history;

    bool ParseKernelMessage(const std::string& raw_line, std::string& timestamp, std::string& message);
    std::string ReadDmesgLine();
    bool ProcessHistoryLog(RawMessage& raw_msg);
    bool ProcessRealtimeLog(RawMessage& raw_msg);

public:
    KernelSink();
    ~KernelSink();

    virtual void Init() override;
    virtual void Exit() override;
    virtual bool Poll(RawMessage& raw_msg) override;
};