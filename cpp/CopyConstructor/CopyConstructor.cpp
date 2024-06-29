// CopyConstructor.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

#include "CTestData.h"

using namespace std;

// 매개변수가 CTestData 클래스의 '참조' 형식이므로 객체가 생성되지 않는다.
void TestFunc(const CTestData &param)
{
    cout << "TestFunc()" << endl;

    cout << "m_nData: " << param.GetData() << endl;
}

int main()
{
    cout << "*****Begin*****" << endl;
    CTestData a(10);
    TestFunc(a);

    a.SetData(20);
    TestFunc(a);

    cout << "*****End*****" << endl;

    return 0;
}
