#pragma once

#include <string>
#include <vector>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <condition_variable>

/**
 * @brief A sophisticated Log Buffer Management System designed to optimize data flow and ensure thread safety.
 * 
 * @note This innovative buffer class incorporates a dual-level buffer architecture, comprising L1 and L2 buffers.
 * 
 * @note The data flow adheres to the following paradigm:
 * @note socket -> class Monitor -> L1 buffer -> L2 buffer -> .log file
 * 
 * @note To guarantee uninterrupted write operations by the Monitor, the L1 buffer employs a double-buffering technique,
 * @note effectively eliminating any potential mutex-related impediments.
 * 
 * @note A dedicated thread is responsible for seamlessly transferring data from L1 to L2 via the Transcription() method.
 * 
 * @note The quintessential usage pattern involves a sequence of push, transcription, and pull operations.
 * 
 * @ref "/test/case/socket/monitor.hpp"
 */
class Buffer 
{
private:
    std::vector<std::string> l1_buffer_1;
    std::vector<std::string> l1_buffer_2;
    std::vector<std::string> l2_buffer;
    size_t l1_capacity;
    size_t l2_capacity;
    size_t l1_head_1;
    size_t l1_tail_1;
    size_t l1_head_2;
    size_t l1_tail_2;
    size_t l2_head;
    size_t l2_tail;
    std::mutex l1_mtx_1;
    std::mutex l1_mtx_2;
    std::mutex l2_mtx;
    std::mutex transcription_mtx;
    std::condition_variable l2_cv;
    bool use_first_l1;
    std::thread transcription_thread;
    bool stop_transcription;

public:
    Buffer(size_t l1_capacity, size_t l2_capacity);

    ~Buffer();

    void Push(const std::string& log);

    void Transcription();

    std::string Pull(const std::chrono::milliseconds& timeout);

    bool IsL2Empty();

private:
    void TranscriptionLoop();
};