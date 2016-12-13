/****************************************************************************
*    espripharness is an application for the Raspberry Pi which allows
*    testing of an ESP-12 microcontroller module
*
*    Copyright riban 2016. Licence GPL V3.
*
*****************************************************************************/
#include "utestgpi.h"
#include <unistd.h> //provides sleep
#include <string>
#include <sstream> //provides ostringstream

UTestGPI::UTestGPI(RPIESP* esp) :
    UnitTest::UnitTest(esp)
{
    m_sName = "GPI";
}

UTestGPI::~UTestGPI()
{
}

bool UTestGPI::RunTest(unsigned int test)
{
    unsigned int pins[] = {0,1,2,3,4,5,12,13,14,15,16,20,21};
    ostringstream ss;
    unsigned int nCount;
    switch(test)
    {
        case 1:
            for(unsigned int nIndex = 0; nIndex < 13; ++nIndex)
            {
                ss << pins[nIndex];
                string sGPIO = "GPIO" + ss.str();
                m_pEsp->out(sGPIO, HIGH);
                sleep(1);
                m_pEsp->out(sGPIO, LOW);
                sleep(1);
                m_pEsp->enableUart();
            }
            break;
        case 2:
            for(unsigned int nIndex = 0; nIndex < 13; ++nIndex)
            {
                ss << pins[nIndex];
                string sGPIO = "GPIO" + ss.str();
                //Wait for up to 1 second for pin to go high
                nCount = 0;
                while(m_pEsp->in(sGPIO) != HIGH)
                {
                    if(nCount++ > 10) //poll for transition every 100ms
                    {
                        //Timed out waiting so fail and break out of while loop
                        m_bPass = false;
                        break;
                    }
                    usleep(10000);
                }
                if(!m_bPass)
                {
                    //!@todo log failure
                    break; //previous test stage failed so break out of for loop
                }
                //Wait for up to 1 second for pin to go low
                nCount = 0;
                while(m_pEsp->in(sGPIO) != LOW)
                {
                        //Timed out waiting so fail and break out of while loop
                    if(nCount++ > 10) //poll for transition every 100ms
                    {
                        m_bPass = false;
                        break;
                    }
                    usleep(10000);
                }
            }
            m_pEsp->enableUart();
            break;
        default:
            ;
    }
    return true;
}
