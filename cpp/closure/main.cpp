#include <iostream>

#ifdef CLOSURE_LAMBDA

auto createCounter = [](int add_value) {
    return [count = 0, add_value]() mutable {
        count += add_value;
        std::cout << count << std::endl;
    };
};

#else

class createCounter
{
public:
    createCounter(int add_value)
        : add_value(add_value)
    {
    }

    void operator()() const
    {
        count += add_value;
        std::cout << count << std::endl;
    }

private:
    mutable int count = 0;
    int add_value;
};

#endif

int main(void)
{
    auto counterBy1 = createCounter(1);
    auto counterBy3 = createCounter(3);

    counterBy1();
    counterBy1();

    counterBy3();
    counterBy3();

    return 0;
}
