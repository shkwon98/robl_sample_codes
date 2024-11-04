#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

// Get the output of the command "dpkg -l | grep copilot"
// the expected output is: "ii  copilot  0.2.1-rc.16  arm64  copilot debian package"
// Return the version number as 0x0002010a
std::uint32_t GetRunningVersion()
{
    auto exec = [](const std::string &cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);

        if (!pipe)
        {
            return result;
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        {
            result += buffer.data();
        }
        return result;
    };

    const std::string result = exec("dpkg -l | grep copilot");
    if (result.empty())
    {
        return 0;
    }

    std::string line;

    while (std::getline(std::istringstream(result), line))
    {
        std::istringstream iss(line);
        std::string deco_str, pkg_name, version;
        iss >> deco_str >> pkg_name >> version;

        if (pkg_name == "copilot")
        {
            std::uint32_t major, minor, patch, rc;
            if (std::sscanf(version.c_str(), "%u.%u.%u-rc.%u", &major, &minor, &patch, &rc) == 4)
            {
                return (major << 24) | (minor << 16) | (patch << 8) | rc;
            }
        }
    }

    return 0;
}

int main()
{
    auto version = GetRunningVersion();

    std::cout << "Major: " << ((version >> 24) & 0xFF) << std::endl;
    std::cout << "Minor: " << ((version >> 16) & 0xFF) << std::endl;
    std::cout << "Patch: " << ((version >> 8) & 0xFF) << std::endl;
    std::cout << "RC: " << (version & 0xFF) << std::endl;
    return 0;
}