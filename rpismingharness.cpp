/****************************************************************************
*    espripharness is an application for the Raspberry Pi which allows
*    testing of an ESP-12 microcontroller module
*
*    Copyright riban 2016
*
*****************************************************************************/

#define VERSION "0.0.1"

#include <iostream>
#include <getopt.h>
#include "rpiesp.h"

using namespace std;

enum MODE
{
    MODE_NONE,
    MODE_RESET,
    MODE_FLASH,
    MODE_RUN,
    MODE_POWERDOWN,
    MODE_INFO
};
bool g_bVerbose = false;

int main(int argc, char* argv[])
{
    unsigned int nMode = MODE_NONE;
    //Command line options - configure in this structure and getopt_long call in while loop
    static struct option long_options[] =
    {
	{"help", no_argument, 0, 'h'},
        {"verbose", no_argument, 0, 'V'},
        {"version", no_argument, 0, 'v'},
        {"reset", no_argument, 0, 'R'},
        {"run", no_argument, 0, 'r'},
        {"flash", no_argument, 0, 'f'},
        {"powerdown", no_argument, 0, 'p'},
        {0,0,0,0}
    };
    int option_index = 0;
    int nAction = 0;
    while((nAction = getopt_long(argc, argv, "hfprRvV", long_options, &option_index)) != -1)
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
                break;
            case 'v':
                cout << "Version "<< VERSION << endl;
                nMode = MODE_INFO;
                break;
            case 'p':
                nMode = MODE_POWERDOWN;
                break;
            default:
                break;
        }

    }
    RPIESP esp;
    esp.enableDebug();
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
    if(nMode != MODE_NONE)
        return 0;
    return 0;
}
