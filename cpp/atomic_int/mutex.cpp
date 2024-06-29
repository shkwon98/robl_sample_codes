#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

void worker(int &counter, std::mutex &mutex)
{
    for (int i = 0; i < 1000000; i++)
    {
        std::lock_guard<std::mutex> lock(mutex);
        counter++;
    }
}

int main()
{
    int counter = 0;
    std::mutex mutex;

    std::vector<std::thread> workers;
    for (int i = 0; i < 4; i++)
    {
        workers.push_back(std::thread(worker, std::ref(counter), std::ref(mutex)));
    }

    for (int i = 0; i < 4; i++)
    {
        workers[i].join();
    }

    std::cout << "Counter 최종 값 : " << counter << std::endl;
}