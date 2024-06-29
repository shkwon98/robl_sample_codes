#include <iostream>
#include <unordered_map>


int main(int argc, char** argv)
{
    std::unordered_map<std::string, int> map;
    map["a"] = 1;
    map["b"] = 2;
    map["c"] = 3;
    map["d"] = 4;
    map["e"] = 5;

    auto it = map.find("a");
    auto order = std::distance(map.begin(), it) + 1;
    std::cout << "order: " << order << std::endl;

    it = map.find("b");
    order = std::distance(map.begin(), it) + 1;
    std::cout << "order: " << order << std::endl;

    it = map.find("c");
    order = std::distance(map.begin(), it) + 1;
    std::cout << "order: " << order << std::endl;

    it = map.find("d");
    order = std::distance(map.begin(), it) + 1;
    std::cout << "order: " << order << std::endl;

    it = map.find("e");
    order = std::distance(map.begin(), it) + 1;
    std::cout << "order: " << order << std::endl;

    return 0;
}