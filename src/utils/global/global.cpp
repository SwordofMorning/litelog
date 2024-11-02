#include "global.h"

std::atomic<long> g_kernel_uptime(0);
char g_real_time[64] = {0};
std::mutex g_time_mutex;