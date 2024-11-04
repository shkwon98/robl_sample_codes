#include <iostream>

static int global = 0;

class A
{
public:
    void Add()
    {
        static int local = 0;

        std::cout << "local: " << ++local << std::endl;
        std::cout << "member: " << ++member << std::endl;
        std::cout << "global: " << ++global << std::endl << std::endl;
    }

    void Sub()
    {
        static int local = 0;

        std::cout << "local: " << --local << std::endl;
        std::cout << "member: " << --member << std::endl;
        std::cout << "global: " << --global << std::endl << std::endl;
    }

private:
    static int member;
};

int A::member = 0;

int main()
{
    A a;
    A b;

    a.Add();
    b.Add();

    a.Sub();
    b.Sub();

    return 0;
}