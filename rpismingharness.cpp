/****************************************************************************
*    espripharness is an application for the Raspberry Pi which allows
*    testing of an ESP-12 microcontroller module
*
*    Copyright riban 2016. Licence GPL V3.
*
*****************************************************************************/

#define VERSION "0.0.1"

#include "rpismingharness.h"
#include "rpiesp.h"
#include "logger.h"
#include <getopt.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdarg.h> //provides va_* functions
#include <fstream> //provides fopen, etc.
#include <ctime> //provides time functions
#include <unistd.h> //provides sleep
#include <stdio.h>
#include <sys/time.h> //provides time / select functions

//Include header for each unit test
#include "utestgpi.h"

using namespace std;

enum MODE
{
    MODE_NONE,
    MODE_RESET,
    MODE_FLASH,
    MODE_RUN,
    MODE_POWERDOWN,
    MODE_TEST,
    MODE_INFO
};

bool g_bVerbose = false;
Logger g_logger("rpismingharness.log");
RPIESP esp;

int main(int argc, char* argv[])
{
    string sTest;
    unsigned int nMode = MODE_TEST;
    string sConfFilename = "rpismingharness.conf";

    //Command line options - configure in this structure and getopt_long call in while loop
    static struct option long_options[] =
    {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"config", required_argument, 0, 'c'},
        {"verbose", no_argument, 0, 'V'},
        {"reset", no_argument, 0, 'R'},
        {"run", no_argument, 0, 'r'},
        {"flash", no_argument, 0, 'f'},
        {"powerdown", no_argument, 0, 'p'},
        {"test", required_argument, 0, 't'},
        {0,0,0,0}
    };
    int option_index = 0;
    int nAction = 0;
    while((nAction = getopt_long(argc, argv, "c:hfprRt:vV", long_options, &option_index)) != -1)
    {
        if(nAction == -1)
            break;
        int nOption = 0;
        switch(nAction)
        {
        case 'h':
            cout << "Usage: " << argv[0] << " [options]" << endl;
            while(long_options[nOption].name != 0)
            {
                cout << "\t -" << (char)(long_options[nOption].val) << " --" << long_options[nOption].name << endl;
                ++nOption;
            }
            nMode = MODE_INFO;
            break;
        case 'R':
            //reset
            nMode = MODE_RESET;
            break;
        case 'f':
            //flash
            nMode = MODE_FLASH;
            break;
        case 'r':
            //run
            nMode = MODE_RUN;
            break;
        case 'V':
            //verbose
            g_bVerbose = true;
            esp.enableDebug();
            break;
        case 'v':
            cout << "Version "<< VERSION << endl;
            nMode = MODE_INFO;
            break;
        case 'p':
            nMode = MODE_POWERDOWN;
            break;
        case 'c':
            sConfFilename = optarg;
            break;
        case 't':
            sTest = optarg;
        default:
            break;
        }

    }
    switch(nMode)
    {
    case MODE_RUN:
        esp.run();
        break;
    case MODE_FLASH:
        esp.flash();
        break;
    case MODE_RESET:
        esp.reset();
        break;
    case MODE_POWERDOWN:
        esp.powerdown();
        break;
    default:
        ;
    }
    if(nMode != MODE_TEST)
        return 0;

    //Read configuration file
    ifstream file;
    file.open(sConfFilename.c_str());
    if(file.is_open())
    {
        if(g_bVerbose)
            cout << "Opened configuration file" << endl;
        string sLine, sSection;
        while(getline(file, sLine))
        {
            sLine = sLine.substr(0, sLine.find('#')); //remove comments
            if(sLine[0] == '[')
            {
                //Get section name
                sSection = sLine.substr(1, sLine.find(']') - 1);
                if(g_bVerbose)
                    cout << "Found section '" << sSection << "'" << endl;
                continue;
            }
            //Find key=value pairs
            size_t nPos = sLine.find('=');
            if(nPos == string::npos)
                continue;
            string sParam = sLine.substr(0, nPos);
            string sValue = sLine.substr(nPos + 1);
            sParam.erase(sParam.find_last_not_of(" \t") + 1); //trim (remove leading white space)
            sValue.erase(0, sValue.find_first_not_of(" \t")); //trim (remove trailing white space)
            if(sSection == "PINS")
                //PINS section
                esp.setPin(sParam, atoi(sValue.c_str()));
            else if(sSection == "SERIAL")
            {
                //Serial port section
                if(sParam == "port")
                    esp.getUart()->SetDevice(sValue);
                else if(sParam == "baud")
                    esp.getUart()->SetBaud(String2Int(sValue));
                else if(sParam == "bits")
                    esp.getUart()->SetBits(String2Int(sValue));
                else if(sParam == "parity")
                {
                    switch(sValue[0])
                    {
                        case 'e':
                            esp.getUart()->SetParity(EVEN);
                            break;
                        case 'o':
                            esp.getUart()->SetParity(ODD);
                            break;
                        case 'n':
                            esp.getUart()->SetParity(NONE);
                    }
                }
                else if(sParam == "stop")
                    esp.getUart()->SetStop(String2Int(sValue));
            }
        }
        file.close();
    }
    else
    {
        if(g_bVerbose)
            cout << "Failed to open configuration file '" << sConfFilename << "'" << endl;
    }

    //If we are here then we want to run the unit tests
    //Load tests
    g_vTests.push_back(new UTestGPI(&esp));
    //!@todo Add all unit tests

    //Boot ESP module and wait for it to be ready
    esp.run(); //!@todo Assuming we have correct firmware - should we actually upload it here?
    if(!esp.waitReady())
    {
        cerr << "ESP module not initialised" << endl;
        return -1;
    }
    //Initialise test run
    if(!esp.sendCommand(UART::SOH))
    {
        cerr << "ESP module failed to initialise test run" << endl;
        return -1;
    }
    bool bPass = true;
    //Iterate through each unit test
    for(vector<UnitTest*>::iterator it = g_vTests.begin(); it != g_vTests.end(); ++it)
    {
        UnitTest* pTest = *it;
        if(!sTest.empty())
        {
            if(pTest->GetName() == sTest)
            {
                bPass = runTest(pTest);
                break; //Requested single test be run so now that is done, exit loop
            }
        }
        else
            bPass &= runTest(pTest);
    }
    if(g_bVerbose)
    {
        if(bPass)
            cout << "All tests passed." << endl;
        else
            cout << "Some tests failed!" << endl;
    }
    return bPass?0:-1;
}

bool runTest(UnitTest* pTest)
{
    g_logger.print("Running test %s", pTest->GetName().c_str());
    bool bSuccess = pTest->Run();
    g_logger.print("Test %s %s", pTest->GetName().c_str(), bSuccess?"Passed":"Failed");
    return bSuccess;
}

int String2Int(const string value)
{
    return atoi(value.c_str());
}

