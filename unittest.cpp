/****************************************************************************
*    espripharness is an application for the Raspberry Pi which allows
*    testing of an ESP-12 microcontroller module
*
*    Copyright riban 2016. Licence GPL V3.
*
*****************************************************************************/
#include "unittest.h"
#include <sstream> //provides ostringstream

UnitTest::UnitTest(RPIESP* esp) :
    m_pEsp(esp),
    m_bPass(true),
    m_nSteps(0)
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

unsigned int UnitTest::GetSteps()
{
    return m_nSteps;
}

bool UnitTest::RunStep(unsigned int step)
{
    return true;
}

bool UnitTest::Run()
{
    StartTest();
    for(unsigned int nStep = 0; nStep < m_nSteps; ++nStep)
        m_bPass &= RunStep(nStep);
    return EndTest();
}

bool UnitTest::StartTest()
{
    //Enable UART
    m_pEsp->enableUart();
    //send test#+FS
    ostringstream ss;
    ss << UTEST_ID;
    ss << UART::FS;
    string sTest = ss.str();
    return m_pEsp->sendMessage(sTest);
}

bool UnitTest::EndTest()
{
    //!@todo Check if EndTest needs to do more
    m_pEsp->enableUart();
    return m_bPass;
}

void UnitTest::Reset()
{
    m_bPass = true;
}

void UnitTest::AddStep(TestStep pTestStep)
{
    m_vSteps.push_back(pTestStep);
}
