#include <cstring>
#include <iostream>
#include <semaphore.h>
#include <sys/shm.h>
#include <thread>
#include <vector>

/*--------------------------------------------------------------------*/

#define SHM_SIZE    1024 /* make it a 1K shared memory segment */
#define MAX_READERS 10

/*--------------------------------------------------------------------*/

struct T_DATA
{
    int increment;
};

struct T_SHM_SDM_LOCK
{
    sem_t in;
    sem_t mx;            // 읽는 카운터 보호에 대한 세마포어
    sem_t wrt;           // 보호하려는 공유 자원에 대한 세마포어
    uint32_t read_count; // 읽고 있는 reader 수
};

struct SharedMemory
{
    T_DATA shared_data; // 공유 변수

    T_SHM_SDM_LOCK lock;
};

/*--------------------------------------------------------------------*/

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
        sem_init(&sharedMemory->lock.in, 1, 1);
        sem_init(&sharedMemory->lock.mx, 1, 1);
        sem_init(&sharedMemory->lock.wrt, 1, 1);
    }

    auto reader = [&sharedMemory](int reader_id, bool &is_running) {
        while (is_running)
        {
            // Entry section
            sem_wait(&sharedMemory->lock.in);
            sem_wait(&sharedMemory->lock.mx);
            if (++(sharedMemory->lock.read_count) == 1) // First reader must block writer's access
            {
                sem_wait(&sharedMemory->lock.wrt);
            }
            sem_post(&sharedMemory->lock.mx);
            sem_post(&sharedMemory->lock.in);

            // Critical section
            std::cout << "Reader " << reader_id << ": read shared_data = " << sharedMemory->shared_data.increment
                      << std::endl;

            // Exit section
            sem_wait(&sharedMemory->lock.mx);
            if (--(sharedMemory->lock.read_count) == 0) // Last reader must release writer's access
            {
                sem_post(&sharedMemory->lock.wrt);
            }
            sem_post(&sharedMemory->lock.mx);
        }
    };

    auto writer = [&sharedMemory](int writer_id, bool &is_running) {
        while (is_running)
        {
            // Entry section
            sem_wait(&sharedMemory->lock.in);
            sem_wait(&sharedMemory->lock.wrt);

            // Critical section
            sharedMemory->shared_data.increment++;
            std::cout << "Writer: " << writer_id << ": wrote shared_data = " << sharedMemory->shared_data.increment
                      << std::endl;

            // Exit section
            sem_post(&sharedMemory->lock.wrt);
            sem_post(&sharedMemory->lock.in);

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

    sem_destroy(&sharedMemory->lock.in);
    sem_destroy(&sharedMemory->lock.mx);
    sem_destroy(&sharedMemory->lock.wrt);

    /* detach from the segment: */
    if (shmdt(data) == -1)
    {
        perror("shmdt");
        exit(1);
    }

    return 0;
}