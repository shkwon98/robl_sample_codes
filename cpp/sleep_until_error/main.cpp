#include <chrono>
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <vector>

class StarveFreeLock
{
public:
    StarveFreeLock() noexcept
        : reader_count_(0)
        , writer_waiting_count_(0)
        , writing_(false)
    {
    }

    void lock_shared() noexcept
    {
        std::unique_lock<std::mutex> lock(mutex_);
        reader_cv_.wait(lock, [this]() { return writer_waiting_count_ == 0 && !writing_; });
        ++reader_count_;
    }

    void unlock_shared() noexcept
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (--reader_count_ == 0 && writer_waiting_count_ > 0)
        {
            writer_cv_.notify_one();
        }
    }

    void lock() noexcept
    {
        std::unique_lock<std::mutex> lock(mutex_);
        ++writer_waiting_count_;
        writer_cv_.wait(lock, [this]() { return reader_count_ == 0 && !writing_; });
        --writer_waiting_count_;
        writing_ = true;
    }

    void unlock() noexcept
    {
        std::unique_lock<std::mutex> lock(mutex_);
        writing_ = false;
        if (writer_waiting_count_ > 0)
        {
            writer_cv_.notify_one();
        }
        else
        {
            reader_cv_.notify_all();
        }
    }

private:
    std::mutex mutex_;
    std::condition_variable reader_cv_;
    std::condition_variable writer_cv_;
    int reader_count_;
    int writer_waiting_count_;
    bool writing_;
};

using namespace std::chrono_literals;

void writer(int i, StarveFreeLock &rw_lock)
{
    while (true)
    {
        const auto &now = std::chrono::system_clock::now();

        rw_lock.lock();
        std::cout << "Writer " << i << " is writing" << std::endl;
        std::this_thread::sleep_until(now + 10ms);
        rw_lock.unlock();

        std::this_thread::sleep_until(now + 100ms);
    }
}

void reader(StarveFreeLock &rw_lock)
{
    while (true)
    {
        const auto &now = std::chrono::system_clock::now();

        rw_lock.lock_shared();
        std::cout << "Reader is reading" << std::endl;
        std::this_thread::sleep_until(now + 10ms);
        rw_lock.unlock_shared();

        std::this_thread::sleep_until(now + 100ms);
    }
}

int main()
{
    StarveFreeLock rw_lock;

    std::vector<std::thread> readers;
    std::vector<std::thread> writers;

    // Create readers
    for (int i = 0; i < 10; ++i)
    {
        readers.push_back(std::thread(reader, std::ref(rw_lock)));
    }

    // Create writers
    for (int i = 0; i < 5; ++i)
    {
        writers.push_back(std::thread(writer, i, std::ref(rw_lock)));
    }

    // Join readers
    for (auto &reader : readers)
    {
        reader.join();
    }

    // Join writers
    for (auto &writer : writers)
    {
        writer.join();
    }

    return 0;
}