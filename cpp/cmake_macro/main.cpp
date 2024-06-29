#include <iostream>

#ifndef PKG_VERSION
#define PKG_VERSION "Unknown"
#endif

int main(int argc, char** argv)
{
    std::cout << "Version: " << PKG_VERSION << std::endl;
    return 0;
}