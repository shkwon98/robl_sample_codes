#include <iostream>
#include <memory>

std::unique_ptr<int> GetIntPtr(bool cond)
{
    auto ret_ptr = std::unique_ptr<int>();
    if (cond)
    {
        ret_ptr = std::make_unique<int>();
        *ret_ptr = 42;
    }

    return ret_ptr;
}

int main(void)
{
    if (GetIntPtr(false) != nullptr)
    {
        std::cout << "ptr is not null" << std::endl;
    }
    else
    {
        std::cout << "ptr is null" << std::endl;
    }

    return 0;
}