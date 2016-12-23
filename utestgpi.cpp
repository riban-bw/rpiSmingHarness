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

static const unsigned int PINS[] = {0,1,2,3,4,5,12,13,14,15,16,20,21}; //ESP GPIO pins

UTestGPI::UTestGPI(RPIESP* esp) :
    UnitTest::UnitTest(esp)
{
    m_sName = "GPI";
    //This feels messy. We have to fully qualify the function, get its address then cast it as type (TestStep) before passing to AddStep().
    //It feels like there should be a simpler method (without resorting to macros).
    AddStep((TestStep)(&UTestGPI::Step1));
    AddStep((TestStep)(&UTestGPI::Step2));
}

UTestGPI::~UTestGPI()
{
}

bool UTestGPI::Step1()
{
    ostringstream ss;
    for(unsigned int nIndex = 0; nIndex < 13; ++nIndex)
    {
        ss << PINS[nIndex];
        string sGPIO = "GPIO" + ss.str();
        m_pEsp->out(sGPIO, HIGH);
        sleep(1);
        m_pEsp->out(sGPIO, LOW);
        sleep(1);
        m_pEsp->enableUart();
    }
    return true; //!@todo get result from ESP module
}

bool UTestGPI::Step2()
{
    bool bPass = true;
    ostringstream ss;
    unsigned int nCount;
    for(unsigned int nIndex = 0; nIndex < 13; ++nIndex)
    {
        ss << PINS[nIndex];
        string sGPIO = "GPIO" + ss.str();
        //Wait for up to 1 second for pin to go high
        nCount = 0;
        while(m_pEsp->in(sGPIO) != HIGH)
        {
            if(nCount++ > 10) //poll for transition every 100ms
            {
                //Timed out waiting so fail and break out of while loop
                bPass = false;
                break;
            }
            usleep(10000);
        }
        if(!bPass)
        {
            //!@todo log failure
            break; //previous step failed so break out of for loop
        }
        //Wait for up to 1 second for pin to go low
        nCount = 0;
        while(m_pEsp->in(sGPIO) != LOW)
        {
                //Timed out waiting so fail and break out of while loop
            if(nCount++ > 10) //poll for transition every 100ms
            {
                bPass = false;
                break;
            }
            usleep(10000);
        }
    }
    m_bPass &= bPass;
    return bPass;
}
