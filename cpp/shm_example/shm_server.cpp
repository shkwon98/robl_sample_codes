#include <cstdint>
#include <cerrno>
#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
// #include <pthread.h>

int main()
{
    int shmid;
    void *addr;

    char c;
    uint32_t key = 0xD0D0D0D0;
    uint32_t size = 27;
    uint32_t flag;
    char *s;

    /* Create the segment.  */
    if ((shmid = shmget(key, size, 0666)) < 0)
    {
        perror("shmget");
        return -1;
    }

    /* Now we attach the segment to our data space.  */
    if ((addr = shmat(shmid, nullptr, 0)) == (void *)-1)
    {
        perror("shmat");
        return -1;
    }

    /* Now put some things into the memory for the
     * other process to read.  */
    s = (char *)addr;

    for (c = 'a'; c <= 'z'; c++)
        *s++ = c;
    *s = NULL;

    /*
     * Finally, we wait until the other process
     * changes the first character of our memory
     * to '*', indicating that it has read what
     * we put there.
     */
    while (*(char *)addr != '*')
        sleep(1);

    return 0;
}