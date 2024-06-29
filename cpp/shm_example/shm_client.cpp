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

    uint32_t key = 0xD0D0D0D0;
    uint32_t size = 27;

    char *s;

    /* Locate the segment.  */
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

    /* Now read what the server put in the memory.  */
    for (s = (char *)addr; *s != NULL; s++)
        putchar(*s);
    putchar('\n');

    /*
     * Finally, change the first character of the
     * segment to '*', indicating we have read
     * the segment.
     */
    *(char *)addr = '*';

    return 0;
}