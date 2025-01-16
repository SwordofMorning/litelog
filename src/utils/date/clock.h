#pragma once

#include <thread>
#include <atomic>
#include <chrono>
#include <time.h>
#include <functional>
#include <stdexcept>
#include "../global/global.h"

class Clock
{
private:
    Clock();
    ~Clock();
    Clock(const Clock&) = delete;
    void operator=(const Clock&) = delete;

    static std::unique_ptr<Clock, std::function<void(Clock*)>> m_instance;

    std::thread m_update_thread;
    std::atomic<bool> m_stop_flag;

    void UpdateTime();
    void UpdateLoop();

public:
    static void Start();
    static void Stop();
    static Clock& Get_Instance();
};