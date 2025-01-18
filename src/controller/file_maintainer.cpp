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
    try
    {
        if (!std::filesystem::exists(m_log_path))
        {
            if (!std::filesystem::create_directories(m_log_path))
            {
                throw std::runtime_error("Failed to create directory: " + m_log_path);
            }
        }
        else
        {
            if (!std::filesystem::is_directory(m_log_path))
            {
                throw std::runtime_error(m_log_path + " exists but is not a directory");
            }

            auto perms = std::filesystem::status(m_log_path).permissions();
            if ((perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none)
            {
                throw std::runtime_error("No write permission for directory: " + m_log_path);
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        throw std::runtime_error("Filesystem error: " + std::string(e.what()));
    }
}

void FileMaintainer::Exit()
{
    Clean();
}

std::vector<std::filesystem::path> FileMaintainer::GetLogFiles() const
{
    std::vector<std::filesystem::path> log_files;

    try
    {
        for (const auto& entry : std::filesystem::directory_iterator(m_log_path))
        {
            try
            {
                if (entry.is_regular_file() && entry.path().extension() == ".log")
                {
                    log_files.push_back(entry.path());
                }
            }
            catch (const std::filesystem::filesystem_error&)
            {
                continue;
            }
        }

        // clang-format off
        std::sort(log_files.begin(), log_files.end(),
            [](const std::filesystem::path& a, const std::filesystem::path& b) {
                try {
                    return std::filesystem::last_write_time(a) > std::filesystem::last_write_time(b);
                }
                catch (const std::filesystem::filesystem_error&)
                {
                    return false;
                }
            });
        // clang-format on
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return std::vector<std::filesystem::path>();
    }

    return log_files;
}

void FileMaintainer::Clean()
{
    std::lock_guard<std::mutex> lock(m_maintain_mutex);

    try
    {
        if (!std::filesystem::exists(m_log_path) || !std::filesystem::is_directory(m_log_path))
        {
            return;
        }

        auto log_files = GetLogFiles();

        if (log_files.size() > m_file_nums)
        {
            for (size_t i = m_file_nums; i < log_files.size(); ++i)
            {
                try
                {
                    if (std::filesystem::exists(log_files[i]))
                    {
                        std::filesystem::remove(log_files[i]);
                    }
                }
                catch (const std::filesystem::filesystem_error& e)
                {
                    continue;
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        return;
    }
}

void FileMaintainer::operator()()
{
    while (!m_stop_maintain)
    {
        {
            std::unique_lock<std::mutex> lock(m_maintain_mutex);
            if (m_cv.wait_for(lock, std::chrono::minutes(1), [this] {
                    return m_stop_maintain.load();
                }))
            {
                break;
            }
        }

        Clean();
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
    if (!m_maintainer)
        return;

    {
        std::lock_guard<std::mutex> lock(m_maintainer->m_maintain_mutex);
        m_maintainer->m_stop_maintain = true;
    }
    m_maintainer->m_cv.notify_one();
}

FileMaintainer& FileMaintainer::Get_Instance()
{
    if (!m_maintainer)
    {
        throw std::runtime_error("FileMaintainer is not initialized");
    }
    return *m_maintainer.get();
}