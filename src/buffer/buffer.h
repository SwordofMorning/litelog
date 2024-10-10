#pragma once

#include <string>
#include <vector>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <condition_variable>

class Buffer {
private:
    std::vector<std::string> l1_buffer;
    std::vector<std::string> l2_buffer;
    size_t l1_capacity;
    size_t l2_capacity;
    size_t l1_head;
    size_t l1_tail;
    size_t l2_head;
    size_t l2_tail;
    std::mutex mtx;
    std::condition_variable cv;

public:
    Buffer(size_t l1_capacity, size_t l2_capacity)
        : l1_buffer(l1_capacity), l2_buffer(l2_capacity),
          l1_capacity(l1_capacity), l2_capacity(l2_capacity),
          l1_head(0), l1_tail(0), l2_head(0), l2_tail(0) {}

    void PutToL1(const std::string& log) {
        std::unique_lock<std::mutex> lock(mtx);
        l1_buffer[l1_head] = log;
        l1_head = (l1_head + 1) % l1_capacity;
        if (l1_head == l1_tail) {
            l1_tail = (l1_tail + 1) % l1_capacity;
        }
        cv.notify_one();
    }

    void CopyFromL1ToL2() {
        std::unique_lock<std::mutex> lock(mtx);
        while (l1_head != l1_tail) {
            l2_buffer[l2_head] = l1_buffer[l1_tail];
            l2_head = (l2_head + 1) % l2_capacity;
            if (l2_head == l2_tail) {
                l2_tail = (l2_tail + 1) % l2_capacity;
            }
            l1_tail = (l1_tail + 1) % l1_capacity;
        }
    }

    std::string GetFromL2() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return l2_head != l2_tail; });
        std::string log = l2_buffer[l2_tail];
        l2_tail = (l2_tail + 1) % l2_capacity;
        return log;
    }

    bool IsL2Empty() {
        std::unique_lock<std::mutex> lock(mtx);
        return l2_head == l2_tail;
    }
};