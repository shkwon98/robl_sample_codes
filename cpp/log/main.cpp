#include <iostream>
#include <string>

namespace ami
{

class Logger
{
public:
    Logger(void)
    {
    }

#define PrintLog(message) PrintLog_(__FILE__, __LINE__, __FUNCTION__, message)
    void PrintLog_(const std::string &file, int line, const std::string &function, const std::string &message)
    {
        std::cout << file << "(" << line << ") " << function << ": " << message << "\n";
    }

#define LogString(message) LogString_(__FILE__, __LINE__, __FUNCTION__, message)
    std::string LogString_(const std::string &file, int line, const std::string &function, const std::string &message)
    {
        return file + "(" + std::to_string(line) + ") " + function + ": " + message + "\n";
    }
};
} // namespace ami

int main()
{
    ami::Logger().PrintLog("Hello world!");
    std::cout << "log occured!!!: " << ami::Logger().LogString("Hello world!");

    return 0;
}