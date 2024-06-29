#include <cmath>
#include <iostream>

int main(void)
{
    float nan_val = 32.1f / 0.0f;
    std::cout << "nan_val: " << nan_val << std::endl;

    uint32_t casted_val = nan_val;
    std::cout << "casted_val: " << casted_val << std::endl;

    return 0;
}