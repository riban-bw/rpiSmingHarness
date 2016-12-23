#pragma once
#include <map>
#include <string>
#include <iostream>
#include <bcm2835.h>
#include "uart.h"

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

        /** @brief  Wait up to 10s for ESP module to complete initialisation (expect "RPiSmingHarness READY" to be last message)
        *   @return <i>bool</i> True if"RPiSmingHarness READDY" received within 10s
        *   @note   Checks ten times, once every second. Blocks until success or timeout
        */
        bool waitReady();

        /** @brief  Send command to ESP module
        *   @param  command The command to send
        *   @return <i>bool</i> True on success
        */
        bool sendCommand(unsigned char command);

        /** @brief  Send string to ESP module
        *   @param  message The string to send
        *   @return <i>bool</i> True on success
        */
        bool sendMessage(string message);

        /** @brief  Wait up to 1 second for an acknowledment message on serial port
        *   @return <i>bool</i> True if ACK recieved within one second
        *   @note   Checks 10 times, once each 100ms
        */
        bool getAck();

        /** @brief  Get a pointer to the UART
        *   @return <i>UART*</i> Pointer to UART object
        *   @note   Allows parent class to share UART and access UART functions directly
        *   @note   RPIESP class opens and closes UART - do not assume it will be in same state as last used.
        *   @note   This function allows setting of UART configuration, e.g. baud
        */
        UART* getUart();

    protected:
    private:
        bool openUart(unsigned int baud = 9600, unsigned int bits = 8, PARITY parity = EVEN, unsigned int stop = 1); //Opens the serial port. Returns true on success
        void closeUart(); //Closes the serial port.
        bool m_bDebug; //True to enable debug output
        bool m_bInit; //True if bcm2835 library is initialised
        map<string,unsigned int> m_mmPin; //Map of ESP module functions to BCM2835 pins: m_mmPin[ESP Function] = BCM2835 pin
        UART m_uart; //Serial port
};
