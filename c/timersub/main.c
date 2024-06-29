#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int main(void)
{
    struct timeval start = { 1, 123 * 1000 };
    struct timeval end = { 0, 200 * 1000 };
    struct timeval temp;

    timersub(&start, &end, &temp);

    printf("temp = %ld.%06ld\n", temp.tv_sec, temp.tv_usec);

    return 0;
}