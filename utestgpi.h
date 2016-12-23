/****************************************************************************
*    espripharness is an application for the Raspberry Pi which allows
*    testing of an ESP-12 microcontroller module
*
*    Copyright riban 2016. Licence GPL V3.
*
*****************************************************************************/
#pragma once
#include "unittest.h"
#include <string>
using namespace std;

/** GPI Unit Test
*   Step 1: Asserts for 1s then clears for 1s each GPIO pin, one at a time, starting with lowest index.
*   Step 2: Monitors GPIO pins, expecting each to be asserted for 1s then cleared for 1s, starting with lowest index.
*   @note   Serial port is unavailable during (part) of this test. Serial port is restored at end of each test
*/
class UTestGPI : public UnitTest
{
    public:
        UTestGPI(RPIESP* esp);
        virtual ~UTestGPI();

    protected:

    private:
        static const unsigned int UTEST_ID = 1; //Unit Test ID - should this be defined by the main application?
//        static const unsigned int PINS[] = {0,1,2,3,4,5,12,13,14,15,16,20,21};
        bool Step1(); //First step
        bool Step2(); //Second step
};
