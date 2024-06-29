#include <stdio.h>

typedef struct
{
    int		x;
    int		y;
} T_NAS_INF_REC;

typedef struct
{
    __uint32_t		nas_inf_rec_no;
    T_NAS_INF_REC	nas_inf_rec[0];
} D__INF__NAS_INF_INFO;


int main()
{
    // D__INF__NAS_INF_INFO INFO;

    printf("%ld\n", sizeof(T_NAS_INF_REC));
    printf("%ld\n", sizeof(D__INF__NAS_INF_INFO));

    return 0;
}
