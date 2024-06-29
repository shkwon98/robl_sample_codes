#include <iostream>
#include <string>

class CharToStr
{
public:
    // template <typename ArgT0>
    // void SetStr(ArgT0 &&arg0)
    // {
    //     str_ = std::string(std::forward<ArgT0>(arg0));
    // }

    void SetStr(const char *c_str)
    {
        str_ = std::string(c_str);
    }

    std::string GetStr(void) const
    {
        return str_;
    }

private:
    std::string str_;
};

int main(void)
{
    char c_str[16] = "Hello, World!";

    CharToStr char_to_str;

    char_to_str.SetStr(c_str);
    std::cout << "1. char_to_str: " << char_to_str.GetStr() << std::endl;

    // char_to_str.SetStr(std::string(c_str));
    // std::cout << "2. char_to_str: " << char_to_str.GetStr() << std::endl;

    return 0;
}