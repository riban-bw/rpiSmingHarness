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
#include <vector>

using namespace std;


/** @brief  Abstract class providing framework for unit tests within the espRpiHarness application
*   @note   Each test has a name which may be called by the application running on the ESP module via its serial connection to the RPi
*   @note   Each test may have several steps. These steps may be requested individually but the common workflow is to call Run() function to run all steps.
*   @note   The derived class should add a function for each step and add a pointer to each of these step functions using AddStep(*function).
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

        /** @brief  Get the quantity of steps in this this unit test
        *   @return <i>unsigned int</i> Quantity of steps
        */
        unsigned int GetSteps();

        /** @brief  Runs specified step
        *   @param  test Index of the step to run [0,1,...] (Default: 0 (first step))
        *   @return <i>bool</i> True on success
        */
        virtual bool RunStep(unsigned int test = 0);

        /** @brief  Runs all tests
        *   @return <i>bool</i> True on success
        */
        bool Run();

        /** @brief  Resets the unit test
        */
        void Reset();

        /** @brief  Instruct ESP module to start the unit test
        *   @return <i>bool</i> True on success. May fail if ESP module not connected or running wrong firmware
        */
        bool StartTest();

        /** @brief  Resets RPi to command mode and reports success of unit test
        *   @return <i>bool</i> True on success. May fail if any steps of unit test fail or ESP module not responding correctly
        *   @note   Call at end of unit test.
        */
        bool EndTest();

        /** @brief  Get the unit test ID
        *   @return <i>unsigned int</i>Unit Test ID
        */
        unsigned int GetID() { return UTEST_ID; }

    protected:
        typedef bool(UnitTest::*TestStep)(); //This defines the function type for individual test steps
        void AddStep(TestStep pTestStep);
        string m_sName; //Name of unit test
        RPIESP* m_pEsp; //Pointer to the ESP module object
        bool m_bPass; //True on unit test pass
        static const unsigned int UTEST_ID = 1; //Unique identifier for each type of unit test

    private:
        vector<UnitTest::TestStep> m_vSteps; //List of functions that form the steps of the unit test
        unsigned int m_nSteps; //Quantity of tests
};
