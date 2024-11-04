#include <algorithm>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <vector>

class MountMonitor
{
public:
    explicit MountMonitor()
        : is_monitoring_(true)
        , is_mounted_(false)
        , monitor_thread_(&MountMonitor::Monitor, this)
    {
    }
    ~MountMonitor()
    {
        is_monitoring_ = false;
        monitor_thread_.join();
    }

    bool IsMounted()
    {
        return is_mounted_;
    }

    void SetMounted(bool mounted)
    {
        is_mounted_ = mounted;
    }

    template <class F, class... Args>
    std::size_t RegisterMountCallback(F &&f, Args &&...args)
    {
        auto callback = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

        std::scoped_lock lock(mount_callbacks_mutex_);
        mount_callbacks_.push_back(callback);

        return mount_callbacks_.size() - 1;
    }

    template <class F, class... Args>
    std::size_t RegisterUnmountCallback(F &&f, Args &&...args)
    {
        auto callback = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

        std::scoped_lock lock(unmount_callbacks_mutex_);
        unmount_callbacks_.push_back(callback);

        return unmount_callbacks_.size() - 1;
    }

    void CancelMountCallback(std::size_t index)
    {
        std::scoped_lock lock(mount_callbacks_mutex_);
        if (index < mount_callbacks_.size())
        {
            mount_callbacks_[index] = nullptr; // Mark as null for safe removal later
        }
    }

    void CancelUnmountCallback(std::size_t index)
    {
        std::scoped_lock lock(unmount_callbacks_mutex_);
        if (index < unmount_callbacks_.size())
        {
            unmount_callbacks_[index] = nullptr; // Mark as null for safe removal later
        }
    }

private:
    void Monitor()
    {
        bool last_mounted_state = false;

        while (is_monitoring_)
        {
            bool current_mounted_state = IsMounted();

            if (current_mounted_state && !last_mounted_state)
            {
                // Mounted: Run mount callbacks
                std::vector<std::future<void>> futures;
                {
                    std::scoped_lock lock(mount_callbacks_mutex_);
                    for (const auto &callback : mount_callbacks_)
                    {
                        if (callback)
                        {
                            // Run each callback asynchronously
                            futures.push_back(std::async(std::launch::async, callback));
                        }
                    }

                    // Remove nullified callbacks after running
                    mount_callbacks_.erase(std::remove(mount_callbacks_.begin(), mount_callbacks_.end(), nullptr),
                                           mount_callbacks_.end());
                }

                // Wait for all asynchronous callbacks to finish
                for (auto &future : futures)
                {
                    future.get(); // Block until each future completes
                }
            }
            else if (!current_mounted_state && last_mounted_state)
            {
                // Unmounted: Run unmount callbacks
                std::vector<std::future<void>> futures;
                {
                    std::scoped_lock lock(unmount_callbacks_mutex_);
                    for (const auto &callback : unmount_callbacks_)
                    {
                        if (callback)
                        {
                            // Run each callback asynchronously
                            futures.push_back(std::async(std::launch::async, callback));
                        }
                    }

                    // Remove nullified callbacks after running
                    unmount_callbacks_.erase(std::remove(unmount_callbacks_.begin(), unmount_callbacks_.end(), nullptr),
                                             unmount_callbacks_.end());
                }

                // Wait for all asynchronous callbacks to finish
                for (auto &future : futures)
                {
                    future.get(); // Block until each future completes
                }
            }

            last_mounted_state = current_mounted_state;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    std::atomic<bool> is_monitoring_;
    std::atomic<bool> is_mounted_;
    std::thread monitor_thread_;

    std::mutex mount_callbacks_mutex_;
    std::vector<std::function<void()>> mount_callbacks_;

    std::mutex unmount_callbacks_mutex_;
    std::vector<std::function<void()>> unmount_callbacks_;
};

int main(void)
{
    MountMonitor monitor;

    auto mount_callback = []() { std::cout << "Mounted!" << std::endl; };
    auto unmount_callback = []() { std::cout << "Unmounted!" << std::endl; };

    auto id1 = monitor.RegisterMountCallback(mount_callback);
    auto id2 = monitor.RegisterMountCallback(mount_callback);

    auto id3 = monitor.RegisterUnmountCallback(unmount_callback);
    auto id4 = monitor.RegisterUnmountCallback(unmount_callback);

    monitor.SetMounted(true);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    monitor.SetMounted(false);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    monitor.CancelMountCallback(id1);
    monitor.CancelUnmountCallback(id3);

    monitor.SetMounted(true);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    monitor.SetMounted(false);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
