#include <fcntl.h> /* For O_* constants */
#include <iostream>
#include <mutex>
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

#define SHM_SIZE 1024 /* make it a 1K shared memory segment */

struct SharedMemory
{
    int a;
    std::mutex mtx;
    sem_t sem;
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
        sem_init(&sharedMemory->sem, 0, 1);
    }

    std::thread t1([&sharedMemory]() {
        for (int i = 0; i < 100000000; i++)
        {
            std::lock_guard<std::mutex> lock(sharedMemory->mtx);
            sharedMemory->a++;
        }
    });
    std::thread t2([&sharedMemory]() {
        for (int i = 0; i < 100000000; i++)
        {
            std::lock_guard<std::mutex> lock(sharedMemory->mtx);
            sharedMemory->a++;
        }
    });

    t1.join();
    t2.join();

    std::cout << sharedMemory->a << std::endl;

    /* detach from the segment: */
    if (shmdt(data) == -1)
    {
        perror("shmdt");
        exit(1);
    }

    return 0;
}