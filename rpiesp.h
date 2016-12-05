#pragma once
#include <map>
#include <string>
#include <iostream>

using namespace std;

class RPIESP
{
    public:

        RPIESP();
        virtual ~RPIESP();

        /** @brief  Set debug output
        *   @param  enable True to enable debug output, false to disable (default: true)
        */
        void enableDebug(bool enable = true);

        /** @brief  Set the RPi GPIO connected to the esp module pin
        *   @param  espPin The name of the esp module pin
        *   @param  rpiGPIO The name of the RPi GPIO
        *   @return <i>bool</i> True on success
        */
        bool setPin(string espPin, unsigned int rpiGPIO);

        /** @brief  Reset the esp module
        */
        void reset();

        /** @brief  Put the esp module into low power mode (power down)
        */
        void powerdown();

        /** @brief  Put the esp module in flash upload mode
        */
        void flash();

        /** @brief  Put the esp module in run mode
        */
        void run();

        /** @brief  Get list of valid ESP module pins
        *   @return <std::string> Comma separated list of valid pins
        */
        string getValidPins();

        /** @brief  Set a GPIO output value
        *   @param  espPin Name of the ESP module pin
        *   @param  value Value to set pin to [LOW | HIGH]
        */
        void out(string espPin, unsigned int value);

    protected:
    private:
        bool m_bDebug; //True to enable debug output
        bool m_bInit; //True if bcm2835 library is initialised
        map<string,unsigned int> m_mmPin; //Map of ESP pin to RPi GPIO
};
