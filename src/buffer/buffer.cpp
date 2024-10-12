#include "buffer.h"

Buffer::Buffer(size_t l1_capacity, size_t l2_capacity)
    : l1_buffer_1(l1_capacity)
    , l1_buffer_2(l1_capacity)
    , l2_buffer(l2_capacity)
    , l1_capacity(l1_capacity)
    , l2_capacity(l2_capacity)
    , l1_head_1(0)
    , l1_tail_1(0)
    , l1_head_2(0)
    , l1_tail_2(0)
    , l2_head(0)
    , l2_tail(0)
    , use_first_l1(true)
    , stop_transcription(false)
{
    transcription_thread = std::thread(&Buffer::TranscriptionLoop, this);
}

Buffer::~Buffer()
{
    stop_transcription = true;
    transcription_thread.join();
}

void Buffer::Push(const std::pair<uint64_t, std::string>& log)
{
    if (use_first_l1)
    {
        std::unique_lock<std::mutex> lock(l1_mtx_1);
        l1_buffer_1[l1_head_1] = log;
        l1_head_1 = (l1_head_1 + 1) % l1_capacity;
        if (l1_head_1 == l1_tail_1)
            l1_tail_1 = (l1_tail_1 + 1) % l1_capacity;
    }
    else
    {
        std::unique_lock<std::mutex> lock(l1_mtx_2);
        l1_buffer_2[l1_head_2] = log;
        l1_head_2 = (l1_head_2 + 1) % l1_capacity;
        if (l1_head_2 == l1_tail_2)
            l1_tail_2 = (l1_tail_2 + 1) % l1_capacity;
    }
}

void Buffer::Transcription()
{
    std::unique_lock<std::mutex> transcription_lock(transcription_mtx);

    if (use_first_l1)
    {
        std::unique_lock<std::mutex> l1_lock(l1_mtx_2, std::defer_lock);
        std::unique_lock<std::mutex> l2_lock(l2_mtx, std::defer_lock);
        std::lock(l1_lock, l2_lock);

        std::vector<std::pair<uint64_t, std::string>> temp_buffer;
        while (l1_head_2 != l1_tail_2)
        {
            temp_buffer.push_back(l1_buffer_2[l1_tail_2]);
            l1_tail_2 = (l1_tail_2 + 1) % l1_capacity;
        }

        std::sort(temp_buffer.begin(), temp_buffer.end());

        for (const auto& entry : temp_buffer)
        {
            l2_buffer[l2_head] = entry.second;
            l2_head = (l2_head + 1) % l2_capacity;
            if (l2_head == l2_tail)
                l2_tail = (l2_tail + 1) % l2_capacity;
        }
    }
    else
    {
        std::unique_lock<std::mutex> l1_lock(l1_mtx_1, std::defer_lock);
        std::unique_lock<std::mutex> l2_lock(l2_mtx, std::defer_lock);
        std::lock(l1_lock, l2_lock);

        std::vector<std::pair<uint64_t, std::string>> temp_buffer;
        while (l1_head_1 != l1_tail_1)
        {
            temp_buffer.push_back(l1_buffer_1[l1_tail_1]);
            l1_tail_1 = (l1_tail_1 + 1) % l1_capacity;
        }

        std::sort(temp_buffer.begin(), temp_buffer.end());

        for (const auto& entry : temp_buffer)
        {
            l2_buffer[l2_head] = entry.second;
            l2_head = (l2_head + 1) % l2_capacity;
            if (l2_head == l2_tail)
                l2_tail = (l2_tail + 1) % l2_capacity;
        }
    }

    use_first_l1 = !use_first_l1;
    l2_cv.notify_one();
}

std::string Buffer::Pull(const std::chrono::milliseconds& timeout)
{
    std::unique_lock<std::mutex> lock(l2_mtx);
    if (!l2_cv.wait_for(lock, timeout, [this] {
            return l2_head != l2_tail;
        }))
        return "";
    std::string log = l2_buffer[l2_tail];
    l2_tail = (l2_tail + 1) % l2_capacity;
    return log;
}

bool Buffer::IsL2Empty()
{
    std::unique_lock<std::mutex> lock(l2_mtx);
    return l2_head == l2_tail;
}

void Buffer::TranscriptionLoop()
{
    while (!stop_transcription)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        Transcription();
    }
}