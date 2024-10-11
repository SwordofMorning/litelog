#pragma once

#include <string>
#include <map>
#include <functional>
#include <iomanip>
#include <queue>
#include <thread>
#include <condition_variable>

class ThreadPool
{
public:
    ThreadPool(size_t num_threads);
    ~ThreadPool();

    void Enqueue(std::function<void()> task);

private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_stop;
};