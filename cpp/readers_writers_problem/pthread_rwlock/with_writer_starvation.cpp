#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <thread>
#include <unistd.h>
#include <vector>

#define SHM_NAME    "/my_shared_memory"
#define SHM_SIZE    sizeof(SharedMemory)
#define MAX_READERS 10

struct SharedMemory
{
    int increment;
    pthread_rwlock_t rwlock;
};

int main(int argc, char *argv[])
{
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd, SHM_SIZE) == -1)
    {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    void *shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    SharedMemory *sharedMemory = (SharedMemory *)shm_ptr;
    pthread_rwlockattr_t rwlock_attr;

    if (argc != 1)
    {
        memset(sharedMemory, 0, SHM_SIZE);
        pthread_rwlockattr_init(&rwlock_attr);
        pthread_rwlockattr_setpshared(&rwlock_attr, PTHREAD_PROCESS_SHARED);
        pthread_rwlock_init(&sharedMemory->rwlock, &rwlock_attr);
    }

    auto reader = [&sharedMemory](int reader_id, bool &is_running) {
        while (is_running)
        {
            pthread_rwlock_rdlock(&sharedMemory->rwlock);
            std::cout << "Reader: " << reader_id << ": read shared_data = " << sharedMemory->increment << std::endl;
            pthread_rwlock_unlock(&sharedMemory->rwlock);
        }
    };

    auto writer = [&sharedMemory](int writer_id, bool &is_running) {
        while (is_running)
        {
            pthread_rwlock_wrlock(&sharedMemory->rwlock);
            sharedMemory->increment++;
            std::cout << "Writer: " << writer_id << ": wrote shared_data = " << sharedMemory->increment << std::endl;
            pthread_rwlock_unlock(&sharedMemory->rwlock);

            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
        }
    };

    bool is_running = true;
    std::vector<std::thread> readers;
    for (int i = 0; i < MAX_READERS; i++)
    {
        readers.push_back(std::thread(reader, i, std::ref(is_running)));
    }

    std::vector<std::thread> writers;
    for (int i = 0; i < 3; i++)
    {
        writers.push_back(std::thread(writer, i, std::ref(is_running)));
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));
    is_running = false;

    for (auto &reader : readers)
    {
        reader.join();
    }
    for (auto &writer : writers)
    {
        writer.join();
    }

    if (argc != 1)
    {
        pthread_rwlockattr_destroy(&rwlock_attr);
    }

    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);

    return 0;
}
