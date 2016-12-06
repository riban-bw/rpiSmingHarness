#include "rpiesp.h"
#include "bcm2835.h"

RPIESP::RPIESP()
{
    m_bInit = bcm2835_init();
    m_bDebug = false;
    m_bInit = false;
    //Map default bcm2835 ins to ESP module pins / functions
    m_mmPin["RESET"]    = 21;
    m_mmPin["CH_PD"]    = 25;
    m_mmPin["GPIO0"]    = 23;
    m_mmPin["GPIO1"]    = 15;
    m_mmPin["GPIO2"]    = 22;
    m_mmPin["GPIO3"]    = 14;
    m_mmPin["GPIO4"]    = 0;
    m_mmPin["GPIO5"]    = 1;
    m_mmPin["GPIO12"]   = 10;
    m_mmPin["GPIO13"]   = 9;
    m_mmPin["GPIO14"]   = 11;
    m_mmPin["GPIO15"]   = 8;
    m_mmPin["GPIO16"]   = 4;
    m_mmPin["ADC"]      = 18;
    //Map RPi GPIO to BCM2835 pins
    m_mmBCM2835Pin[2] = 0;
    m_mmBCM2835Pin[3] = 1;
    m_mmBCM2835Pin[4] = 4;
    m_mmBCM2835Pin[7] = 7;
    m_mmBCM2835Pin[8] = 8;
    m_mmBCM2835Pin[9] = 9;
    m_mmBCM2835Pin[10] = 10;
    m_mmBCM2835Pin[11] = 11;
    m_mmBCM2835Pin[14] = 14;
    m_mmBCM2835Pin[15] = 15;
    m_mmBCM2835Pin[17] = 17;
    m_mmBCM2835Pin[18] = 18;
    m_mmBCM2835Pin[22] = 22;
    m_mmBCM2835Pin[23] = 23;
    m_mmBCM2835Pin[24] = 24;
    m_mmBCM2835Pin[25] = 25;
    m_mmBCM2835Pin[27] = 21;
}

RPIESP::~RPIESP()
{
    bcm2835_close();
}

void RPIESP::enableDebug(bool enable)
{
    m_bDebug = enable;
}

bool RPIESP::setPin(string espPin, unsigned int rpiGPIO)
{
    map<unsigned int, unsigned int>::iterator it = m_mmBCM2835Pin.find(rpiGPIO);
    if(m_mmBCM2835Pin.end() == it)
    {
        if(m_bDebug)
            cerr << "Failed to set ESP pin '" << espPin << "' to RPi 'GPIO" << rpiGPIO << "'- invalid RPi GPIO." << endl;
        return false;
    }
    if(m_mmPin.end() == m_mmPin.find(espPin))
    {
        if(m_bDebug)
            cerr << "Failed to set ESP pin '" << espPin << "' to RPi 'GPIO" << rpiGPIO << " - invalid ESP pin. Valid ESP pins are: " << getValidPins() << endl;
        return false;
    }
    if(m_bDebug && m_mmPin[espPin] != it->second)
        cerr << "Setting " << espPin << " to RPi GPIO" << rpiGPIO << endl;
    m_mmPin[espPin] = it->second;
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
    {
        cerr << "RPIESP not initialised." << endl;
        return;
    }
    //!@todo Implement reset
    if(m_bDebug)
      cerr << "RPIESP::reset" << endl;
      //!@todo Should we use out() instead of bcm2835_gpio_write
    bcm2835_gpio_write(m_mmPin["RESET"], LOW);
    bcm2835_delay(100);
    bcm2835_gpio_write(m_mmPin["RESET"], HIGH);
}

void RPIESP::powerdown()
{
    if(!m_bInit)
    {
        cerr << "RPIESP not initialised." << endl;
        return;
    }
    //!@todo Implement powerdown
    if(m_bDebug)
      cerr << "RPIESP::powerdown" << endl;
    bcm2835_gpio_write(m_mmPin["CH_PD"], LOW);
}

void RPIESP::flash()
{
    if(!m_bInit)
    {
        cerr << "RPIESP not initialised." << endl;
        return;
    }
    //!@todo Implement flash
    if(m_bDebug)
      cerr << "RPIESP::flash" << endl;
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
    {
        cerr << "RPIESP not initialised." << endl;
        return;
    }
    if(m_bDebug)
      cerr << "RPIESP::run" << endl;
    bcm2835_gpio_write(m_mmPin["GPIO15"], LOW);
    bcm2835_gpio_write(m_mmPin["GPIO2"], HIGH);
    bcm2835_gpio_write(m_mmPin["GPIO0"], HIGH);
    bcm2835_delay(100);
    bcm2835_gpio_write(m_mmPin["CH_PD"], HIGH);
}

void RPIESP::out(string espPin, unsigned int value)
{
    if(!m_bInit)
    {
        cerr << "RPIESP not initialised." << endl;
        return;
    }
    if(m_mmPin.end() == m_mmPin.find(espPin))
      return;
    bcm2835_gpio_write(m_mmPin[espPin], value);
}
