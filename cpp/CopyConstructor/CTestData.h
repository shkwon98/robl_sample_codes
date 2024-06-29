#pragma once

#include <iostream>

using namespace std;

class CTestData
{
public:
    CTestData(int nParam);
    CTestData(const CTestData &rhs);
    ~CTestData();

    int GetData() const;
    void SetData(int nParam);

private:
    int m_nData = 0;
};
