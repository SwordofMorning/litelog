#include "file_maintainer.h"

std::unique_ptr<FileMaintainer, std::function<void(FileMaintainer*)>> FileMaintainer::m_maintainer = nullptr;

FileMaintainer::FileMaintainer(const std::string& log_path, size_t file_nums)
    : m_log_path(log_path)
    , m_file_nums(file_nums)
    , m_stop_maintain(false)
{
    Init();
}

FileMaintainer::~FileMaintainer()
{
    Exit();
}

void FileMaintainer::Init()
{
    std::filesystem::create_directories(m_log_path);
}

void FileMaintainer::Exit()
{
    Clean();
}

std::vector<std::filesystem::path> FileMaintainer::GetLogFiles() const
{
    std::vector<std::filesystem::path> log_files;

    for (const auto& entry : std::filesystem::directory_iterator(m_log_path))
    {
        if (entry.path().extension() == ".log")
        {
            log_files.push_back(entry.path());
        }
    }

    // sort by modify time
    // clang-format off
    std::sort(log_files.begin(), log_files.end(),
        [](const std::filesystem::path& a, const std::filesystem::path& b) {
            return std::filesystem::last_write_time(a) > std::filesystem::last_write_time(b);
        });
    // clang-format on

    return log_files;
}

void FileMaintainer::Clean()
{
    std::lock_guard<std::mutex> lock(m_maintain_mutex);

    auto log_files = GetLogFiles();

    if (log_files.size() > m_file_nums)
    {
        for (size_t i = m_file_nums; i < log_files.size(); ++i)
        {
            std::filesystem::remove(log_files[i]);
        }
    }
}

void FileMaintainer::operator()()
{
    while (!m_stop_maintain)
    {
        Clean();
        // check per minutes
        std::this_thread::sleep_for(std::chrono::minutes(1));
    }
}

std::function<void()> FileMaintainer::Start(const std::string& log_path, size_t file_nums)
{
    // clang-format off
    if (!m_maintainer)
        m_maintainer = std::unique_ptr<FileMaintainer, std::function<void(FileMaintainer*)>>
            (new FileMaintainer(log_path, file_nums), [](FileMaintainer* maintainer) { delete maintainer; });
    // clang-format on
    return std::bind(&FileMaintainer::operator(), &(*m_maintainer));
}

void FileMaintainer::Stop()
{
    if (m_maintainer)
    {
        m_maintainer->m_stop_maintain = true;
    }
}

FileMaintainer& FileMaintainer::Get_Instance()
{
    if (!m_maintainer)
    {
        throw std::runtime_error("FileMaintainer is not initialized");
    }
    return *m_maintainer.get();
}