#include <iostream>
#include <memory>

class A
{
public:
    A()
    {
        std::cout << "A constructor" << std::endl;
    }
    ~A()
    {
        std::cout << "A destructor" << std::endl;
    }

    void SetNumber(int number)
    {
        number_ = number;
    }
    int GetNumber() const
    {
        return number_;
    }

private:
    int number_ = 0;
};

class B
{
public:
    B(const std::shared_ptr<A> &a)
        : a_(a)
    {
        std::cout << "B constructor" << std::endl;
        a->SetNumber(20);
        a_->SetNumber(30);
    }
    ~B()
    {
        std::cout << "B destructor" << std::endl;
    }

private:
    std::shared_ptr<A> a_;
};

class MyClass
{
public:
    MyClass()
    {
        a_ = std::make_shared<A>();
        std::cout << "a_.use_count() = " << a_.use_count() << std::endl;
        std::cout << "number = " << a_->GetNumber() << std::endl;

        a_->SetNumber(10);
        std::cout << "a_.use_count() = " << a_.use_count() << std::endl;
        std::cout << "number = " << a_->GetNumber() << std::endl;

        b_ = std::make_shared<B>(a_);
        std::cout << "a_.use_count() = " << a_.use_count() << std::endl;
        std::cout << "number = " << a_->GetNumber() << std::endl;
    }

private:
    std::shared_ptr<A> a_;
    std::shared_ptr<B> b_;
};

int main(void)
{
    MyClass my_class;

    return 0;
}
