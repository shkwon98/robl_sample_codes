#include <iostream>

class Parent
{
public:
    Parent(int &ref_int)
        : ref_int_(ref_int)
    {
        std::cout << "Parent constructor" << std::endl;
        std::cout << "Parent ref_int_ = " << ref_int_ << std::endl;
    }
    ~Parent()
    {
        std::cout << "Parent destructor" << std::endl;
    }

private:
    int &ref_int_;
};

class Child : public Parent
{
public:
    Child(int &ref_int)
        : Parent(ref_int)
    {
        std::cout << "Child constructor" << std::endl;
    }
    ~Child()
    {
        std::cout << "Child destructor" << std::endl;
    }
};

int main()
{
    int a = 10;
    Child child(a);
    return 0;
}