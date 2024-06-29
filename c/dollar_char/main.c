#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

#define PWGEN_PATH      "./pwgen"
#define PW_SUFFIX       "avikusssh"
#define PW_KEY_BUF_SIZE (6 + strlen(PW_SUFFIX) + 1) // 6(mac) + strlen(suffix) + null
#define PW_LENGTH       (32)

int main(void)
{
    char password[PW_LENGTH + 1];
    memset(password, 0, PW_LENGTH + 1);
    strcpy(password, "EA$$$w1<");

    // password에서 '$' 문자가 있으면 '$' 문자를 '\$'로 변경
    size_t password_length = strlen(password);
    for (size_t i = 0; i < password_length; i++)
    {
        if (password[i] == '$')
        {
            memmove(password + i + 1, password + i, password_length - i + 1);
            password[i] = '\\';
            password_length++;
            i++;
        }
    }

    char command[BUFFER_SIZE];
    memset(command, 0, BUFFER_SIZE);
    sprintf(command, "echo \"%s:%s\" | chpasswd", "root", password);

    printf("command: %s\n", command);

    return 0;
}