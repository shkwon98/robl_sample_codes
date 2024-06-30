#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_SIZE 100000

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <number of threads>\n", argv[0]);
        exit(1);
    }

    int num_threads = strtol(argv[1], NULL, 10);

    int a[VECTOR_SIZE];
    int b[VECTOR_SIZE];
    int c[VECTOR_SIZE];

    for (int i = 0; i < VECTOR_SIZE; i++)
    {
        a[i] = 1;
        b[i] = 2;
    }

    int num_elements_per_thread = VECTOR_SIZE / num_threads;
    int *start = (int *)malloc(num_threads * sizeof(int));
    int *end = (int *)malloc(num_threads * sizeof(int));

    printf("Work decomposition\n");
    for (int tid = 0; tid < num_threads; tid++)
    {
        start[tid] = tid * num_elements_per_thread;
        end[tid] = (tid + 1) * num_elements_per_thread;

        if (tid == num_threads - 1)
        {
            end[tid] = VECTOR_SIZE;
        }

        printf("Thread %d: start = %d, end = %d\n", tid, start[tid], end[tid]);
    }

#pragma omp parallel num_threads(num_threads)
    {
        int tid = omp_get_thread_num();

        for (int i = start[tid]; i < end[tid]; i++)
        {
            c[i] = a[i] + b[i];
        }
    }

    printf("Vector sum\n");
    for (int i = 0; i < VECTOR_SIZE; i++)
    {
        printf("c[%d] = %d\n", i, c[i]);
    }

    free(start);
    free(end);

    return 0;
}