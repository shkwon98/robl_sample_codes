#include <iostream>
#include <memory>

std::unique_ptr<int> GetIntPtr()
{
    auto ret_ptr = std::unique_ptr<int>();

    ret_ptr = std::make_unique<int>();
    *ret_ptr = 42 + rand() % 100;

    return ret_ptr;
}

int main(void)
{
    for (int i = 0; i < 10; i++)
    {
        auto ptr = GetIntPtr();
        auto int_ptr = ptr.get();
        // auto int_ptr = GetIntPtr().get();
        std::cout << "int_ptr: " << *int_ptr << std::endl;
    }

    return 0;
}