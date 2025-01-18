#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <filesystem>
#include <condition_variable>

class FileMaintainer
{
private:
    FileMaintainer(const std::string& log_path, size_t file_nums);
    ~FileMaintainer();
    FileMaintainer() = delete;
    void operator=(const FileMaintainer&) = delete;

    std::string m_log_path;
    size_t m_file_nums;
    std::atomic<bool> m_stop_maintain;
    std::mutex m_maintain_mutex;
    std::condition_variable m_cv;

    static std::unique_ptr<FileMaintainer, std::function<void(FileMaintainer*)>> m_maintainer;

    void Init();
    void Exit();
    void Clean();
    void operator()();

    std::vector<std::filesystem::path> GetLogFiles() const;

public:
    static std::function<void()> Start(const std::string& log_path, size_t file_nums);
    static void Stop();
    static FileMaintainer& Get_Instance();
};