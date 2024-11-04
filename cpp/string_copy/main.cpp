#include <iostream>
#include <string>

int main()
{
    std::string str = "Hello, World!!";
    char cstr[14] = { 0 };

    std::cout << "before copy: " << cstr << std::endl;

    auto cnt = str.copy(cstr, sizeof(cstr) - 1);

    std::cout << "after copy: " << cstr << std::endl;
    std::cout << "count: " << cnt << std::endl;

    return 0;
}