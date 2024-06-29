#include <stdint.h>
#include <stdio.h>

int main(void)
{
    uint32_t i_val_1 = 1;

    printf("1. i_val_1: %u\n", i_val_1);
    printf("2. !i_val_1: %u\n", !i_val_1);

    uint32_t i_val_0 = 0;
    printf("3. i_val_0: %u\n", i_val_0);
    printf("4. !i_val_0: %u\n", !i_val_0);

    uint32_t i_val_1234 = 1234;
    printf("5. i_val_1234: %u\n", i_val_1234);
    printf("6. !i_val_1234: %u\n", !i_val_1234);

    uint32_t test_val = 0;
    printf("test_val: %u\n", test_val);

    test_val = !test_val;
    printf("test_val: %u\n", test_val);

    return 0;
}