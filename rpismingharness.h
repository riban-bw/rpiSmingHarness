/****************************************************************************
*    espripharness is an application for the Raspberry Pi which allows
*    testing of an ESP-12 microcontroller module
*
*    Copyright riban 2016. Licence GPL V3.
*
*****************************************************************************/

/** Each unit test is implemented as a derived class from the UnitTest base class.
*   The RPi and ESP module communicate via their serial ports to control tests and report results.
*   Before and after each test, the serial ports must be enabled to communicate at the configured rate (default: 9600-8-n-1).
*   ESP module reports its start-up routine with text messages. The last message must be "RPiSmingHarness READY". Once started, it listens for instructions from the RPi.
*   The RPi sends a SOH (0x01) to initialise the tests and expects a ACK (0x06) returned from the ESP.
*   The RPi sends the test number to perform as ASCII characters followed by FS (0x1C), e.g. for test 1: 1FS. For test 12, 12FS.
*   The ESP returns a ACK (0x06) then commences the test.
*   At the end of the test, the RPi sends ETB(0x17). The ESP responds with ACK (0x06) if the test passed or NAK (0x15) if the test failed.
*   The RPi continues to send test number, FS for each test until end of last test.
*   After last test, the RPi sends EOT (0x03). The ESP responds with ACK (0x06).
*   Each test may have several steps. These are defined in the unit test and may use physical signalling, e.g. GPI value, serial message, etc. or timing, e.g. expect to last 1s.
*   If using serial message to separate steps, the RPi or ESP should send RS (0x1E) and expect ACK or NAK response to indicate success.
*/
#pragma once
#include <string>
#include "unittest.h"
#include <vector>

using namespace std;

/** @brief  Runs a unit test
*   @param  pTest Pointer to the test to run
*   @return <i>bool</i> True on success
*/
bool runTest(UnitTest* pTest);

/** @brief   Convert string to integer
*   @param  value String value to convert
*   @return <int> Integer value of string (0 if invalid numeric value in string)
*/
int String2Int(const string value);

/** @brief  Instruct ESP module to run a test
*   @param  test ID of the test to run
*   @return <i>bool</i> True on success, i.e. if ESP module responds with ACK
*   @note   Call this function to start the test then run the actual test
*/
bool StartTest(unsigned int test);

vector<UnitTest*> g_vTests; //List of pointers to unit tests
