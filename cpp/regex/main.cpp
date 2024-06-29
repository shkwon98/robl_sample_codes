#include <iostream>
#include <regex>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <mode> <value>" << std::endl;
        return 1;
    }

    // tlv is looks like this: {mode}.i,4,{value}
    // use regex_replace to fill the tlv string
    std::string mode = argv[1];
    std::string value = argv[2];
    std::string tlv = "{mode}.hdg,i,4,{value};";

    if (mode == "AH")
    {
        tlv = std::regex_replace(tlv, std::regex{ "\\{mode\\}" }, "AH");
        tlv = std::regex_replace(tlv, std::regex{ "\\{value\\}" }, value);
        std::cout << tlv << std::endl;
    }
    else if (mode == "SH")
    {
        tlv = std::regex_replace(tlv, std::regex("\\{mode\\}"), "SH");
        tlv = std::regex_replace(tlv, std::regex("\\{value\\}"), value);
        std::cout << tlv << std::endl;
    }
    else
    {
        std::cout << "Invalid mode: " << mode << std::endl;
        return 1;
    }

    return 0;
}