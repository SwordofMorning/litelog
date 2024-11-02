#pragma once

#include <atomic>
#include <string>
#include <mutex>

extern std::atomic<long> g_kernel_uptime;
extern char g_real_time[64];
extern std::mutex g_time_mutex;