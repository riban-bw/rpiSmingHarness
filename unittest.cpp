/****************************************************************************
*    espripharness is an application for the Raspberry Pi which allows
*    testing of an ESP-12 microcontroller module
*
*    Copyright riban 2016. Licence GPL V3.
*
*****************************************************************************/
#include "unittest.h"

UnitTest::UnitTest(RPIESP* esp) :
    m_pEsp(esp),
    m_bPass(true),
    m_nTests(0)
{
}

UnitTest::~UnitTest()
{
}

string UnitTest::GetName()
{
    return m_sName;
}

bool UnitTest::IsPassing()
{
    return m_bPass;
}

unsigned int UnitTest::GetTests()
{
    return m_nTests;
}

bool UnitTest::RunTest(unsigned int test)
{
    return true;
}

bool UnitTest::RunAll()
{
    for(unsigned int nTest = 0; nTest < m_nTests; ++nTest)
        m_bPass &= RunTest(nTest);
    return m_bPass;
}

void UnitTest::Reset()
{
    m_bPass = true;
}
