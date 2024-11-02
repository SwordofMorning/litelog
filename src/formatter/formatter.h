#pragma once

#include <pthread.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <string>
#include <fstream>
#include <mutex>
#include <ctime>
#include <iostream>
#include <functional>
#include <atomic>
#include "../utils/config/config.h"
#include "../buffer/buffer.h"

class Formatter
{
private:
    Formatter(const std::string& log_path, Buffer& buffer, size_t max_log_lines);
    ~Formatter();
    Formatter() = delete;
    void operator=(const Formatter&) = delete;

    std::ofstream m_log_file;
    std::mutex m_file_mutex;

    Buffer& m_buffer;
    std::mutex m_write_mutex;
    std::atomic<bool> m_stop_write;
    size_t m_max_log_lines;
    std::string m_log_path;
    std::atomic<size_t> m_lines_written;

    void Init();
    void Exit();
    void Info(const std::string& str_time);

    static std::unique_ptr<Formatter, std::function<void(Formatter*)>> m_formatter;

    void Write(const std::string& str);
    void operator()();

public:
    static std::function<void()> Start(const std::string& log_path, Buffer& buffer, size_t max_log_lines);
    static void Stop();
    void Switch();

    static Formatter& Get_Instance();
};