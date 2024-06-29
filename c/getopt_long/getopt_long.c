#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/*
struct option {
    const char *name;	// 옵션의 긴 이름
    int	has_arg;		// 옵션에 해당하는 자료 = 이후, 1: 필요, 0:불필요
    int *flag;			// 아래 지정한 값을 받을 수 있는 변수 주소
    int val;			// 이 값을 옵션에 따라 flag 변수에 대입.
};
*/

static struct option long_option[] = {
    // char *name, int has_arg, int *flag, int val
    { "file1", 1, 0, 0 },
    { "num1", 1, 0, 0 },
    { "file2", 1, 0, 0 },
    { "num2", 1, 0, 0 },
    { 0, 0, 0, 0 }
};


int main(int argc, char *argv[])
{
    int c;
    int index;

    char file1[64];
    int num1;
    int num2;
    char file2[64];

    memset(file1, 0x0, sizeof(file1));
    memset(file2, 0x0, sizeof(file2));

    while ((c = getopt_long(argc, argv, "a:b:c:d:", long_option, &index)) != -1)
    {
        switch (c)
        {
            case 0:
                if (strcmp(long_option[index].name, "file1") == 0)
                {
                    strcpy(file1, optarg);
                    printf("file1: %s\n", file1);
                }

                else if (strcmp(long_option[index].name, "num1") == 0)
                {
                    num1 = strtol(optarg, NULL, 16);    // 16진수 문자열을int로
                    printf("num1: 0x%x (%d)\n", num1, num1);
                }

                else if (strcmp(long_option[index].name, "num2") == 0)
                {
                    num2 = strtol(optarg, NULL, 16);
                    printf("num2: 0x%x (%d)\n", num2, num2);
                }

                else if (strcmp(long_option[index].name, "file2") == 0)
                {
                    strcpy(file2, optarg);
                    printf("file2: %s\n", file2);
                }

                break;

            case 'a':
                strcpy(file1, optarg);
                printf("file1: %s\n", file1);
                break;

            case 'b':
                num1 = strtol(optarg, NULL, 16);
                printf("num1: 0x%x (%d)\n", num1, num1);
                break;

            case 'c':
                strcpy(file2, optarg);
                printf("file2: %s\n", file2);
                break;

            case 'd':
                num2 = strtol(optarg, NULL, 16);
                printf("num2: 0x%x (%d)\n", num2, num2);
                break;

            default:
                break;
        }

    }

    return 0;
}