#include <stdint.h>
#include <stdio.h>

#define BUCKET_SIZE   509
#define DID_TOTAL_CNT (27)

int main(void)
{
    uint32_t did_buckets[BUCKET_SIZE][1 + DID_TOTAL_CNT]; // hash table for DID

    uint32_t *p_bucket = &did_buckets[10];
    uint32_t *p_counter = &did_buckets[10][0];

    p_bucket[++(*p_counter)] = 1234;

    printf("did_buckets[10][0] = %d\n", did_buckets[10][0]);
    printf("did_buckets[10][1] = %d\n", did_buckets[10][1]);

    return 0;
}