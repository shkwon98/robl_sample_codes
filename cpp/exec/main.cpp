#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

std::string exec(const char *cmd)
{
    auto buffer = std::array<char, 128>();
    auto result = std::string();
    auto pipe = std::unique_ptr<FILE, decltype(&pclose)>(popen(cmd, "r"), pclose);

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    return result;
}

int main(int argc, char **argv)
{
    std::string output = exec("ls -l");
    std::cout << "Output: " << output << std::endl;
    return 0;
}