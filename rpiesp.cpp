#include "rpiesp.h"
#include <bcm2835.h>

RPIESP::RPIESP()
{
    m_bDebug = false;
    m_bInit = false;
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
    bcm2835_close();
}

void RPIESP::enableDebug(bool enable)
{
    m_bDebug = enable;
    m_bInit = bcm2835_init();
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
    if(!m_bInit)
      return;
    //!@todo Implement reset
    if(m_bDebug)
      cout << "RPIESP::reset" << endl;
    bcm2835_gpio_write(m_mmPin["RESET"], LOW);
    bcm2835_delay(100);
    bcm2835_gpio_write(m_mmPin["RESET"], HIGH);
}

void RPIESP::powerdown()
{
    if(!m_bInit)
      return;
    //!@todo Implement powerdown
    if(m_bDebug)
      cout << "RPIESP::powerdown" << endl;
    bcm2835_gpio_write(m_mmPin["CH_PD"], LOW);
}

void RPIESP::flash()
{
    if(!m_bInit)
      return;
    //!@todo Implement flash
    if(m_bDebug)
      cout << "RPIESP::flash" << endl;
    bcm2835_gpio_write(m_mmPin["RESET"], LOW);
    bcm2835_gpio_write(m_mmPin["GPIO15"], LOW);
    bcm2835_gpio_write(m_mmPin["GPIO2"], HIGH);
    bcm2835_gpio_write(m_mmPin["GPIO0"], LOW);
    bcm2835_gpio_write(m_mmPin["CH_PD"], HIGH);
    bcm2835_delay(100);
    bcm2835_gpio_write(m_mmPin["RESET"], HIGH);
}

void RPIESP::run()
{
    if(!m_bInit)
      return;
    //!@todo Implement run
    if(m_bDebug)
      cout << "RPIESP::run" << endl;
    bcm2835_gpio_write(m_mmPin["GPIO15"], LOW);
    bcm2835_gpio_write(m_mmPin["GPIO2"], HIGH);
    bcm2835_gpio_write(m_mmPin["GPIO0"], HIGH);
    bcm2835_gpio_write(m_mmPin["CH_PD"], HIGH);
}
