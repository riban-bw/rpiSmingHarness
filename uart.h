/** UART - A class providing control of the RPi UART
*   Copyright riban (2016)
*   Author: Brian Walton (brian@riban.co.uk)
*   Licence: LGPL
*/
#pragma once
#include <string>

using namespace std;

enum PARITY
{
    NONE,
    ODD,
    EVEN
};

class UART
{
    public:
        UART();
        virtual ~UART();

        /** @brief  Open the serial port (UART)
        *   @param  baud Baud rate
        *   @param  bits Quantity of data bits (Default: 8)
        *   @param  parity Parity to use [NONE|ODD|EVEN] (Default: NONE)
        *   @param  stop Quantity of stop bits (Default: 1)
        *   @note   RPi GPIO15 is used for Rx and GPIO15 for Tx. These should be connected to ESP Tx & Rx pins.
        */
        bool Open(unsigned int baud = 9600, unsigned int bits = 8, PARITY parity = NONE, unsigned int stop = 1);

        /** @brief  Close the serial port (UART)
        */
        void Close();

        /** @brief  Set the UART device
        *   @brief  Full path of the UART device, e.g. "/dev/ttyAMA0"
        *   @note   Need to close and reopen UART to take effect
        */
        void SetDevice(const string device);

        /** @brief  Set baud rate
        *   @param  baud The baud rate to set
        *   @note   Need to close and reopen UART to take effect
        */
        void SetBaud(unsigned int baud);

        /** @brief  Set data length (bits)
        *   @param  bits The quantity of bits in each data word
        *   @note   Need to close and reopen UART to take effect
        */
        void SetBits(unsigned int bits);

        /** @brief  Set parity
        *   @param  parity The parity to set [NONE|ODD|EVEN]
        *   @note   Need to close and reopen UART to take effect
        */
        void SetParity(PARITY parity);

        /** @brief  Set stop bits
        *   @param  stop The quantity of stop bits
        *   @note   Need to close and reopen UART to take effect
        */
        void SetStop(unsigned int stop);

        /** @brief  Send bytes
        *   @param  buffer Character buffer containing bytes to send
        *   @param  size Quantity of characters to send
        *   @return <i>int</i> Quantity of bytes sent
        *   @note   buffer must be at least 'size' bytes long
        */
        int Send(unsigned char* buffer, size_t size);

        /** @brief  Read bytes
        *   @param  buffer Character buffer to receive bytes
        *   @param  size Maximum quantity of bytes to receive
        *   @return <i>int</i> Quantity of bytes received
        *   @note   buffer must be at least 'size' bytes long
        */
        int Read(unsigned char* buffer, size_t size);

    protected:

    private:
        int m_nFileUart; //Handle for UART0
        string m_sDeviceName; //Full path to UART device
        unsigned int m_nBaud; //Baud rate
        unsigned int m_nBits; //Data word length
        PARITY m_nParity; //Parity
        unsigned int m_nStop; //Stop bits
        void UpdateSettings(); //Apply the current settings to the port if port is open
};
