#include "ami_core.h"

extern T_AMI_DATA ami_data;

void set_package_version(void)
{
    strcpy(ami_data.package_version, PACKAGE_VER);
}

void set_current_version(char *version)
{
    strcpy(ami_data.current_version, version);
}
