#include "rpiesp.h"
#include <bcm2835.h>

RPIESP::RPIESP()
{
    m_bDebug = false;
    m_mmPin["RESET"] = 27;
    m_mmPin["CH_PD"] = 25;
    m_mmPin["GPIO0"] = 23;
    m_mmPin["GPIO1"] = 15;
    m_mmPin["GPIO2"] = 22;
    m_mmPin["GPIO3"] = 14;
    m_mmPin["GPIO4"] = 2;
    m_mmPin["GPIO5"] = 3;
    m_mmPin["GPIO12"] = 10;
    m_mmPin["GPIO13"] = 9;
    m_mmPin["GPIO14"] = 11;
    m_mmPin["GPIO15"] = 8;
    m_mmPin["GPIO16"] = 4;
    m_mmPin["ADC"] = 18;
}

RPIESP::~RPIESP()
{
}

void RPIESP::enableDebug(bool enable)
{
    m_bDebug = enable;

}

bool RPIESP::setPin(string espPin, unsigned int rpiGPIO)
{
    if(rpiGPIO > 27 || m_mmPin.end() == m_mmPin.find(espPin))
    {
        if(m_bDebug)
        {
            cout << "Failed to set ESP pin '" << espPin << "' to RPi 'GPIO" << rpiGPIO << "'. ";
            if(rpiGPIO > 27)
                cout << "RPi GPIO must be between 0-27." << endl;
            else
                cout << "'" << espPin << "' is not a valid ESP pin. Valid ESP pins are: " << getValidPins() << endl;
            return false;
        }
    }
    if(m_bDebug && m_mmPin[espPin] != rpiGPIO)
        cout << "Setting " << espPin << " to RPi GPIO" << rpiGPIO << endl;
    m_mmPin[espPin] = rpiGPIO;
    return true;
}

string RPIESP::getValidPins()
{
    string sReturn;
    for(map<string, unsigned int>::iterator it = m_mmPin.begin(); it != m_mmPin.end(); it++)
    {
        sReturn += it->first;
        sReturn += ", ";
    }
    sReturn.erase(sReturn.length() - 2, 2);
    return sReturn;
}

void RPIESP::reset()
{
    //!@todo Implement reset
    if(m_bDebug)
      cout << "RPIESP::reset" << endl;
}

void RPIESP::powerdown()
{
    //!@todo Implement powerdown
    if(m_bDebug)
      cout << "RPIESP::powerdown" << endl;
}

void RPIESP::flash()
{
    //!@todo Implement flash
    if(m_bDebug)
      cout << "RPIESP::flash" << endl;
}

void RPIESP::run()
{
    //!@todo Implement run
    if(m_bDebug)
      cout << "RPIESP::run" << endl;
}
