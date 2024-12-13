#include <iostream>

#include <boost/thread/future.hpp>

int main()
{
    // recursive then chaining
    boost::promise<int> p1;
    boost::future<int> f1 = p1.get_future();
    auto f2 = f1.then([](boost::future<int> f) { return f.get() + 1; })
                  .then([](boost::future<int> f) { return f.get() + 1; })
                  .then([](boost::future<int> f) { return f.get() + 1; });

    p1.set_value(1);
    std::cout << f2.get() << std::endl;

    // recursive when_all
    boost::promise<int> p2;
    boost::promise<int> p3;
    auto f3 = p2.get_future();
    auto f4 = p3.get_future();
    auto f5 = boost::when_all(f3, f4).then([](boost::future<void> f) { std::cout << "f3 and f4 are ready" << std::endl; });

    p2.set_value(2);
    p3.set_value(3);
    f5.get();

    // recursive when_any
    boost::promise<int> p4;
    boost::promise<int> p5;
    auto f6 = p4.get_future();
    auto f7 = p5.get_future();
    auto f8 = boost::when_any(f6, f7).then([](boost::future<void> f) { std::cout << "f6 or f7 is ready" << std::endl; });

    p4.set_value(4);
    f8.get();

    return 0;
}