#include <fcntl.h> /* For O_* constants */
#include <iostream>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

#define SHM_SIZE    1024 /* make it a 1K shared memory segment */
#define MAX_READERS 10

struct SharedMemory
{
    int shared_data = 0; // 공유 변수
    sem_t rw_mutex;      // 보호하려는 공유 자원에 대한 세마포어
    sem_t mutex;         // 읽는 카운터 보호에 대한 세마포어
    int read_count = 0;  // 읽고 있는 reader 수
};

int main(int argc, char *argv[])
{
    int shmid;
    void *data;
    int mode;

    key_t key = 1234;

    /*  create the segment: */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1)
    {
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    if ((data = shmat(shmid, NULL, 0)) == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }

    SharedMemory *sharedMemory = (SharedMemory *)data;

    if (argc != 1)
    {
        memset(data, 0, SHM_SIZE);
        sem_init(&sharedMemory->rw_mutex, 0, 1);
        sem_init(&sharedMemory->mutex, 0, 1);
    }

    auto reader = [&sharedMemory](int reader_id, bool &is_running) {
        while (is_running)
        {
            // Entry section
            sem_wait(&sharedMemory->mutex);
            sharedMemory->read_count++;
            if (sharedMemory->read_count == 1)
            {
                sem_wait(&sharedMemory->rw_mutex);
            }
            sem_post(&sharedMemory->mutex);

            // Critical section
            std::cout << "Reader " << reader_id << ": read shared_data = " << sharedMemory->shared_data << std::endl;

            // Exit section
            sem_wait(&sharedMemory->mutex);
            sharedMemory->read_count--;
            if (sharedMemory->read_count == 0)
            {
                sem_post(&sharedMemory->rw_mutex);
            }
            sem_post(&sharedMemory->mutex);

            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
        }
    };

    auto writer = [&sharedMemory](int writer_id, bool &is_running) {
        while (is_running)
        {
            // Entry section
            sem_wait(&sharedMemory->rw_mutex);

            // Critical section
            sharedMemory->shared_data++;
            std::cout << "Writer: " << writer_id << ": wrote shared_data = " << sharedMemory->shared_data << std::endl;

            // Exit section
            sem_post(&sharedMemory->rw_mutex);

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

    sem_destroy(&sharedMemory->rw_mutex);
    sem_destroy(&sharedMemory->mutex);

    /* detach from the segment: */
    if (shmdt(data) == -1)
    {
        perror("shmdt");
        exit(1);
    }

    return 0;
}