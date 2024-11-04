#include <filesystem>
#include <iostream>

int main()
{
    std::filesystem::path p = std::filesystem::read_symlink("/proc/self/exe");
    std::cout << p.c_str() << std::endl;

    return 0;
}