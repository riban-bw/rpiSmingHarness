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
*   Test 1: Asserts for 1s then clears for 1s each GPIO pin, one at a time, starting with lowest index.
*   Test 2: Monitors GPIO pins, expecting each to be asserted for 1s then cleared for 1s, starting with lowest index.
*   @note   Serial port is unavailable during (part) of this test. Serial port is restored at end of each test
*/
class UTestGPI : public UnitTest
{
    public:
        UTestGPI(RPIESP* esp);
        virtual ~UTestGPI();

        /** @brief  Runs specified test
        *   @param  test Index of the test to run [0,1,...] (Default: 0 (first test))
        *   @return <i>bool</i> True on success
        */
        bool RunTest(unsigned int test = 0);

    protected:

    private:
};
