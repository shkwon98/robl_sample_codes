#include "CTestData.h"

CTestData::CTestData(int nParam)
    : m_nData(nParam)
{
    cout << "CTestData(int)" << endl;
}

CTestData::CTestData(const CTestData &rhs)
    : m_nData(rhs.m_nData)
{
    cout << "CTestData(const CTestData&)" << endl;
}

CTestData::~CTestData()
{
}

int CTestData::GetData() const
{
    return m_nData;
}

void CTestData::SetData(int nParam)
{
    m_nData = nParam;
}
