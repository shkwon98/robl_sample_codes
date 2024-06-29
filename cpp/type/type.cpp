#include <iostream>
#include <cstdint>
#include <bitset>

using namespace std;


int main()
{
    // problem 1
    float f1 = 65.3;
    float f2 = 0.8;
    cout << "start" << endl;
    cout << endl;
    cout << "float f1 : " << f1 << endl;
    cout << "float f2 : " << f2 << endl;
    cout << endl;
    cout << "f1 + f2 : " << f1 + f2 << endl;
    cout << "(int)f1 + (int)f2 : " << (int)f1 + (int)f2 << endl;
    cout << "(int)(f1 + f2) : " << (int)(f1 + f2) << endl;
    cout << "(char)((int)f1 + (int)f2) : " << (char)((int)f1 + (int)f2) << endl;
    cout << "(char)(f1 + f2) : " << (char)(f1 + f2) << endl;
    cout << endl;
    cout << "end" << endl;

    // problem 2
    typedef union
    {
        uint16_t data16[4];
        struct
        {
            uint16_t member16_1;
            uint16_t member16_2;
            uint16_t member16_3;
            uint16_t member16_4;
        }struct16;

        uint32_t data32[2];
        struct
        {
            uint32_t member32_1;
            uint32_t member32_2;
        }struct32;

        uint64_t data64;

        double data_double;
    }TEST_UNION;

    TEST_UNION test_union;
    test_union.data_double = 65.14159;

    cout << test_union.data_double << endl;
    cout << (float)test_union.data_double << endl;
    cout << bitset<32>(int)test_union.data_double << endl;
    cout << bitset<32>(int)test_union.data32[0] << endl;
    cout << bitset<32>(int)test_union.data32[1] << endl;
    cout << (char)test_union.data_double << endl;
    cout << test_union.data64 << endl;
    cout << bitset<32>test_union.data32[0] << endl;
    cout << test_union.data32[1] << endl;
    cout << test_union.data16[0] << endl;
    cout << test_union.data16[1] << endl;
    cout << test_union.data16[2] << endl;
    cout << test_union.data16[3] << endl;

    return 0;
}