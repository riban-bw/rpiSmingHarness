/** UART - A class providing control of the RPi UART
*   Copyright riban (2016)
*   Author: Brian Walton (brian@riban.co.uk)
*   Licence: LGPL
*/
#include "uart.h"
#include <stdio.h>
#include <unistd.h> //provides UART
#include <fcntl.h> //provides UART
#include <termios.h> //provides terminal control

using namespace std;

UART::UART() :
     m_nFileUart(-1),
     m_sDeviceName("/dev/ttyAMA0"),
     m_nBaud(9600),
     m_nBits(8),
     m_nParity(NONE),
     m_nStop(1)
{
}

UART::~UART()
{
    Close();
}

void UART::SetDevice(const string device)
{
    m_sDeviceName = device;
}

void UART::SetBaud(unsigned int baud)
{
    m_nBaud = baud;
}

void UART::SetBits(unsigned int bits)
{
    m_nBits = bits;
}

void UART::SetParity(PARITY parity)
{
    m_nParity = parity;
}

void UART::SetStop(unsigned int stop)
{
    m_nStop = stop;
}

bool UART::Open(unsigned int baud, unsigned int bits, PARITY parity, unsigned int stop)
{
    open(m_sDeviceName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if(m_nFileUart == -1)
        return false;
	m_nBaud = baud;
	m_nBits = bits;
	m_nParity = parity;
	m_nStop = stop;
	UpdateSettings();
	return true;
}

void UART::Close()
{
    close(m_nFileUart);
    m_nFileUart = -1;
}

void UART::UpdateSettings()
{
    if(m_nFileUart < 0)
        return;
	//Get the settings from the terminal
	struct termios options;
	tcgetattr(m_nFileUart, &options);
    //Adjust each setting (not yet applying the changes)
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	//Apply the change to the terminal with immediate effect (TCSANOW)
	tcsetattr(m_nFileUart, TCSANOW, &options);
}

int UART::Send(unsigned char* buffer, size_t size)
{
    if(m_nFileUart < 0)
        return 0;
    return (write(m_nFileUart, buffer, size));
}

int UART::Read(unsigned char* buffer, size_t size)
{
    if(m_nFileUart < 0)
        return 0;
    return read(m_nFileUart, (void*)buffer, size);
}
