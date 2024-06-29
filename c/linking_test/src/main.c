#include <stdio.h>

#include "ami.h"

T_AMI_DATA ami_data;

int main()
{
    ami_init();
    printf("Package version: %s\n", ami_data.package_version);
    printf("Current version: %s\n", ami_data.current_version);
    return 0;
}