#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char *argv[])
{
    auto WriteResponse = [](int n) { std::cout << "Write Response: " << n << std::endl; };

    auto StartSendingProgress = [&WriteResponse]() {
        auto active = std::make_shared<std::atomic_bool>(true);
        auto thread = std::thread([active, &WriteResponse] {
            while (*active)
            {
                WriteResponse(1);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });

        return std::make_pair(active, std::move(thread));
    };

    auto StopSendingProgress = [](std::pair<std::shared_ptr<std::atomic_bool>, std::thread> &state) {
        auto &[active, thread] = state;

        *active = false;
        if (thread.joinable())
        {
            thread.join();
        }
    };

    auto progress_state = StartSendingProgress();
    std::this_thread::sleep_for(5s);
    StopSendingProgress(progress_state);

    return 0;
}
