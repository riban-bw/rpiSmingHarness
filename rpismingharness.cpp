/****************************************************************************
*    espripharness is an application for the Raspberry Pi which allows
*    testing of an ESP-12 microcontroller module
*
*    Copyright riban 2016
*
*****************************************************************************/

#define VERSION "0.0.1"

#include "rpiesp.h"
#include <getopt.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>

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
    RPIESP esp;
    unsigned int nMode = MODE_NONE;
    string sConfFilename = "rpismingharness.conf";
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
        {"config", required_argument, 0, 'c'},
        {0,0,0,0}
    };
    int option_index = 0;
    int nAction = 0;
    while((nAction = getopt_long(argc, argv, "c:hfprRvV", long_options, &option_index)) != -1)
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
    if(nMode != MODE_NONE)
        return 0;

  ifstream file;
  file.open(sConfFilename.c_str());
  if(file.is_open())
  {
    if(g_bVerbose)
      cout << "Opened configuration file" << endl;
    string sLine, sSection;
    while(getline(file, sLine))
    {
      sLine = sLine.substr(0, sLine.find('#'));
      if(sLine[0] == '[')
      {
        sSection = sLine.substr(1, sLine.find(']') - 1);
        if(g_bVerbose)
          cout << "Found section '" << sSection << "'" << endl;
        continue;
      }
      size_t nPos = sLine.find('=');
      if(nPos == string::npos)
        continue;
      string sParam = sLine.substr(0, nPos);
      string sValue = sLine.substr(nPos + 1);
      sParam.erase(sParam.find_last_not_of(" \t") + 1); //trim
      sValue.erase(0, sValue.find_last_not_of(" \t")); //trim
      if(sSection == "PINS")
        esp.setPin(sParam, atoi(sValue.c_str()));
    }
    file.close();
  }
  else
  {
    if(g_bVerbose)
      cout << "Failed to open configuration file '" << sConfFilename << "'" << endl;
  }

    return 0;
}
