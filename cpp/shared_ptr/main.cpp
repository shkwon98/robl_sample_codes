#include <iostream>
#include <memory>

int main(void)
{
    // auto x = std::make_shared<int>(10);
    // std::cout << "x created..." << std::endl;
    // std::cout << "x = " << *x << ", count = " << x.use_count() << std::endl;
    // {
    //     auto y = x;
    //     std::cout << "y created..." << std::endl;
    //     std::cout << "x = " << *x << ", count = " << x.use_count() << std::endl;
    //     std::cout << "y = " << *y << ", count = " << y.use_count() << std::endl;
    // }
    // std::cout << "y destroyed..." << std::endl;
    // std::cout << "x = " << *x << ", count = " << x.use_count() << std::endl;

    // {
    //     auto z = std::make_shared<int>(20);
    //     std::cout << "z created..." << std::endl;
    //     std::cout << "x = " << *x << ", count = " << x.use_count() << std::endl;
    //     std::cout << "z = " << *z << ", count = " << z.use_count() << std::endl;
    // }

    // {
    //     auto w = x.get();
    //     std::cout << "w created..." << std::endl;
    //     std::cout << "x address = " << x << std::endl;
    //     std::cout << "w address = " << w << std::endl;

    //     x.reset();
    //     std::cout << "x reset..." << std::endl;
    //     std::cout << "xcount = " << x.use_count() << std::endl;
    //     std::cout << "x address = " << x << std::endl;
    //     std::cout << "w address = " << w << std::endl;
    // }

    std::unique_ptr<int> ptr = nullptr;
    std::cout << "ptr created..." << std::endl;
    if (ptr == nullptr)
    {
        std::cout << "ptr is nullptr" << std::endl;
    }

    ptr = std::make_unique<int>(10);
    std::cout << "set ptr..." << std::endl;
    std::cout << "ptr = " << *ptr << std::endl;
    if (ptr == nullptr)
    {
        std::cout << "ptr is nullptr" << std::endl;
    }

    ptr.reset();
    std::cout << "reset ptr..." << std::endl;
    if (ptr == nullptr)
    {
        std::cout << "ptr is nullptr" << std::endl;
    }

    return 0;
}