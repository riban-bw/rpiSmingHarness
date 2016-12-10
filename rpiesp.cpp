#include "rpiesp.h"

RPIESP::RPIESP()
{
    m_bInit = bcm2835_init();
    m_bDebug = false;
    //Map default ESP module functions to BCM2835 pins: m_mmPin[ESP Function] = BCM2835 pin
    m_mmPin["RESET"]    = 27;
    m_mmPin["CH_PD"]    = 25;
    m_mmPin["GPIO0"]    = 23;
    m_mmPin["GPIO1"]    = 15;
    m_mmPin["GPIO2"]    = 22;
    m_mmPin["GPIO3"]    = 14;
    m_mmPin["GPIO4"]    = 2;
    m_mmPin["GPIO5"]    = 3;
    m_mmPin["GPIO12"]   = 10;
    m_mmPin["GPIO13"]   = 9;
    m_mmPin["GPIO14"]   = 11;
    m_mmPin["GPIO15"]   = 8;
    m_mmPin["GPIO16"]   = 4;
    m_mmPin["ADC"]      = 18;
    //Map RPi GPIO to BCM2835 pins: m_mmBCM2835Pin[RPi GPIO#] = BCM2385 pin
    m_mmBCM2835Pin[2] = 2;
    m_mmBCM2835Pin[3] = 3;
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
    m_mmBCM2835Pin[27] = 27;
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
    if(m_bDebug)
      cerr << "RPIESP::reset" << endl;
    out("RESET", LOW);
    bcm2835_delay(100);
    out("RESET", HIGH);
}

void RPIESP::powerdown()
{
    if(m_bDebug)
        cerr << "RPIESP::powerdown" << endl;
    out("CH_PD", LOW);
}

void RPIESP::flash()
{
    if(m_bDebug)
        cerr << "RPIESP::flash" << endl;
    enableUart();
    out("RESET", LOW);
    out("GPIO15", LOW);
    out("GPIO2", HIGH);
    out("GPIO0", LOW);
    out("CH_PD", HIGH);
    bcm2835_delay(100);
    out("RESET", HIGH);
}

void RPIESP::run()
{
    if(m_bDebug)
        cerr << "RPIESP::run" << endl;
    out("CH_PD", LOW);
    out("GPIO15", LOW);
    out("GPIO2", HIGH);
    out("GPIO0", HIGH);
    bcm2835_delay(100);
    out("CH_PD", HIGH);
}

void RPIESP::out(string espPin, unsigned int value)
{
    unsigned int nPin = getPin(espPin);
    if(nPin == BCM_NOT_FOUND)
        return;
    if(m_bDebug)
      cerr << "RPIESP::out - setting ESP module '" << espPin << "' (BCM2835 Pin " << nPin << ") to value " << value << endl;
    mode(espPin, RPIGPIO_OUTPUT);
    bcm2835_gpio_write(nPin, value);
}

uint8_t RPIESP::in(string espPin)
{
    unsigned int nPin = getPin(espPin);
    if(nPin == BCM_NOT_FOUND)
        return 0;
    uint8_t nValue = bcm2835_gpio_lev(nPin);
    if(m_bDebug)
      cerr << "RPIESP::in - ESP module '" << espPin << "' (BCM2835 Pin " << nPin << ") has value " << (int)nValue << endl;
    return nValue;
}

void RPIESP::mode(string espPin, GPIOMODE mode)
{
    unsigned int nPin = getPin(espPin);
    if(nPin == BCM_NOT_FOUND)
        return;
    if(m_bDebug)
      cerr << "RPIESP::mode - ESP module '" << espPin << "' (BCM2835 Pin " << nPin << ") mode set to " << mode << endl;
    bcm2835_gpio_fsel(nPin, mode);
}

unsigned int RPIESP::getPin(string espPin)
{
    if(!m_bInit)
    {
        cerr << "RPIESP not initialised." << endl;
        return BCM_NOT_FOUND;
    }
    if(m_mmPin.end() == m_mmPin.find(espPin))
    {
        cerr << "RPIESP - invalid pin " << espPin << endl;
        return BCM_NOT_FOUND;
    }
    return m_mmPin[espPin];
}

void RPIESP::enableUart()
{
    bcm2835_gpio_fsel(14, BCM2835_GPIO_FSEL_ALT0);
    bcm2835_gpio_fsel(15, BCM2835_GPIO_FSEL_ALT0);
}
