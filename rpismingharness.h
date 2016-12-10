/****************************************************************************
*    espripharness is an application for the Raspberry Pi which allows
*    testing of an ESP-12 microcontroller module
*
*    Copyright riban 2016. Licence GPL V3.
*
*****************************************************************************/
#pragma once
#include <string>

/** @brief  Runs a unit test
*   @param  test Index of the test to run
*   @return <i>bool</i> True on failure
*/
bool runTest(unsigned int test);
