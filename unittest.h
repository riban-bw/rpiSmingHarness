/****************************************************************************
*    espripharness is an application for the Raspberry Pi which allows
*    testing of an ESP-12 microcontroller module
*
*    Copyright riban 2016. Licence GPL V3.
*
*****************************************************************************/
#pragma once
#include "rpiesp.h"
#include <string>

using namespace std;

/** @brief  Abstract class providing framework for unit tests within the espRpiHarness application
*   @note   Each test has a name which may be called by the application running on the ESP module via its serial connection to the RPi
*/
class UnitTest
{
    public:
        UnitTest(RPIESP* esp);
        virtual ~UnitTest();

        /** @brief  Get unit test name
        *   @return <i>std::string</i> Name of unit test
        */
        string GetName();

        /** @brief  Check if unit test is passing
        *   @return <i>bool</i> True if test is passing
        */
        bool IsPassing();

        /** @brief  Get the quantity of tests
        *   @return <i>unsigned int</i> Quantity of tests
        */
        unsigned int GetTests();

        /** @brief  Runs specified test
        *   @param  test Index of the test to run [0,1,...] (Default: 0 (first test))
        *   @return <i>bool</i> True on success
        */
        virtual bool RunTest(unsigned int test = 0);

        /** @brief  Runs all tests
        *   @return <i>bool</i> True on success
        */
        bool RunAll();

        /** @brief  Resets the unit test
        */
        void Reset();

    protected:
        string m_sName; //Name of unit test
        RPIESP* m_pEsp; //Pointer to the ESP module object
        bool m_bPass; //True on unit test pass

    private:
        unsigned int m_nTests; //Quantity of tests
};
