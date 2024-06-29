#include <iostream>

int main(void)
{
    uint32_t a = -1;

    std::cout << "a: " << a << std::endl;
    std::cout << "(bool)a: " << std::boolalpha << static_cast<bool>(a) << std::endl;

    return 0;
}