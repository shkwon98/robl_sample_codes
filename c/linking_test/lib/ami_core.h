#pragma once

#include <string.h>

#ifndef PACKAGE_VER
#define PACKAGE_VER "undef"
#endif

typedef struct
{
    char package_version[8];
    char current_version[8];

} T_AMI_DATA;

void ami_init(void);
