# rpiSmingHarness
A test harness for the Sming framework running on a Raspberry Pi


# Introduction
Sming is a C++ framework for developing applications for the Espressif ESP8266 microcontroller. rpiSmingHarness is an application that allows a Raspberry Pi (RPi) computer to be physically connected to an ESP8266 module and run a series of unit tests to validate Sming is error free. There are a number of unit tests that can be run purely within software but many functions need to be physically tested.

# Physical Connection
rpiSmingHarness is designed to interface with an ESP-12 module which exposes the following ESP8266 pins:

--------
|ESP-12|
--------
|GND   |
|VCC   |
|CH_PD |
|RESET |
|GPIO0 |
|GPIO1 |
|GPIO2 |
|GPIO3 |
|GPIO4 |
|GPIO5 |
|GPIO12|
|GPIO13|
|GPIO14|
|GPIO15|
|GPIO16|
|ADC   |
|------|

These should each be connected to a pin on the RPi connector P1 header. The default mapping is shown below but may be changed by adding entries to a configuration file.

|------|------|
|ESP-12|RPi   |
|------|------|
|GND   |GND   |
|VCC   |3.3V  |
|CH_PD |GPIO25|
|RESET |GPIO27|
|GPIO0 |GPIO23|
|GPIO1 |GPIO15|
|GPIO2 |GPIO22|
|GPIO3 |GPIO15|
|GPIO4 |GPIO0 |
|GPIO5 |GPIO1 |
|GPIO12|GPIO10|
|GPIO13|GPIO9 |
|GPIO14|GPIO11|
|GPIO15|GPIO8 |
|GPIO16|GPIO4 |
|ADC   |GPIO18|
|------|------|

Note: ESP GPIO14 is TxD, ESP GPIO14 is RxD, RPi GPIO1 is TxD, RPi GPIO3 is RxD.
The pin numbers of the RPi and ESP-12 are not shown here because they differ, depending on the version of module.
It is advised that RPi version 2 or later is used because the version 1 unit's 3.3V regulator does not support delivering the 180mA required by the ESP module. Initial tests were however successfully performed with RPi version 1.

# Dependencies
rpiSmingHarness depends on the bcm2835 C library written by Mike McCauley. Follow the instructions for installation at http://www.airspayce.com/mikem/bcm2835/index.html. rpiSmingHarness expects to find "bcm2835.h" in its include path and "libbcm2835.a" in its link path.

# Building
Download or checkout the source code.
Navigate to the source code directory.
Build using `make`.

The Makefile allows building of the application, placing the resulting binary executable in the same directory.

# Usage
