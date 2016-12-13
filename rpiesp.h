#pragma once
#include <map>
#include <string>
#include <iostream>
#include <bcm2835.h>

using namespace std;

typedef enum
{
    RPIGPIO_INPUT   = BCM2835_GPIO_FSEL_INPT,
    RPIGPIO_OUTPUT  = BCM2835_GPIO_FSEL_OUTP,
    RPIGPIO_ALT0    = BCM2835_GPIO_FSEL_ALT0,
    RPIGPIO_ALT1    = BCM2835_GPIO_FSEL_ALT1,
    RPIGPIO_ALT2    = BCM2835_GPIO_FSEL_ALT2,
    RPIGPIO_ALT3    = BCM2835_GPIO_FSEL_ALT3,
    RPIGPIO_ALT4    = BCM2835_GPIO_FSEL_ALT4,
    RPIGPIO_ALT5    = BCM2835_GPIO_FSEL_ALT5
} GPIOMODE;

static const unsigned int BCM_NOT_FOUND = 9999;

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
        *   @note   Sets pin mode to output
        */
        void out(string espPin, unsigned int value);

        /** @brief  Get a GPIO input value
        *   @param  espPin Name of the ESP module pin
        *   @return <i>uint8_t</i> Input value [LOW | HIGH]
        *   @note   Sets pin mode to input
        */
        uint8_t in(string espPin);

        /** @brief  Set GPIO pin mode
        *   @param  espPin Name of the ESP module pin
        *   @param  mode Mode of pin [GPIOMODE]
        */
        void mode(string espPin, GPIOMODE mode);

        /** @brief  Get the BCM2835 connected to an ESP module pin
        *   @param  espPin Name of the ESP module pin
        *   @return <i>unsigned int</i> BCM2835 pin or BCM_NOT_FOUND
        */
        unsigned int getPin(string espPin);

        /** @brief  Configure GPIO 14 & 15 as UART
        */
        void enableUart();

        /** @brief  Sets the value of the PWM output on RPi GPIO18
        *   @param  value Output value [0-1023]
        *   @note   PWM output is at 1172Hz
        */
        void setPwm(unsigned int value);

    protected:
    private:
        bool m_bDebug; //True to enable debug output
        bool m_bInit; //True if bcm2835 library is initialised
        map<string,unsigned int> m_mmPin; //Map of ESP module functions to BCM2835 pins: m_mmPin[ESP Function] = BCM2835 pin
};
