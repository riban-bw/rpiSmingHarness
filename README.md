# rpiSmingHarness
A test harness for the Sming framework running on a Raspberry Pi
# Current status: Alpha
There is very limited functionality currently implemented. Simple control of the ESP module is implemented, e.g. reset, boot in flash mode, power down, etc. Not much else yet. (Early days!)

|Requirement|Status|
|-----------|------|
|Application wire frame|X|
|Reading configuration|X|
|Control of ESP module| X|
|Logging of results|X|
|Reading unit tests from file| |
|Running unit tests| |

---
# Introduction
Sming is a C++ framework for developing applications for the Espressif ESP8266 microcontroller. rpiSmingHarness is an application that allows a Raspberry Pi (RPi) computer to be physically connected to an ESP8266 module and run a series of unit tests to validate Sming is error free. There are a number of unit tests that can be run purely within software but many functions need to be physically tested.

---
# Physical Connection
rpiSmingHarness is designed to interface with an ESP-12 module which exposes the following ESP8266 pins, all of which should be connected to RPi connector P1 pins. The default mapping of ESP-12 to RPi connections is shown in the table but may be changed by adding entries to a configuration file.

|ESP-12|RPi   |RPi Fn|
|------|------|-------|
|GND   |GND   |      |
|VCC   |3.3V  |      |
|CH_PD |GPIO25|      |
|RESET |GPIO27|      |
|GPIO0 |GPIO23|      |
|GPIO1 |GPIO15|RxD   |
|GPIO2 |GPIO22|      |
|GPIO3 |GPIO15|TxD   |
|GPIO4 |GPIO0 |      |
|GPIO5 |GPIO1 |      |
|GPIO12|GPIO10|      |
|GPIO13|GPIO9 |      |
|GPIO14|GPIO11|      |
|GPIO15|GPIO8 |      |
|GPIO16|GPIO4 |      |
|ADC   |GPIO18|PWM   |

The pin numbers of the RPi and ESP-12 are not shown here because they differ, depending on the version of module.
It is advised that RPi version 2 or later is used because the version 1 unit's 3.3V regulator does not support delivering the 180mA required by the ESP module. Initial tests were however successfully performed with RPi version 1.

---
# Dependencies
rpiSmingHarness depends on the bcm2835 C library written by Mike McCauley. Follow the instructions for installation at http://www.airspayce.com/mikem/bcm2835/index.html. rpiSmingHarness expects to find "bcm2835.h" in its include path and "libbcm2835.a" in its link path.

---
# Building
Download or checkout the source code.
Navigate to the source code directory.
Build using `make`.

The Makefile allows building of the application, placing the resulting binary executable in the same directory.

---
# Usage
rpiSmingHarness core purpose is to run automated unit tests of Sming framework but it also provides some useful functions that maybe triggered by passing command line parameters:

|Param|Long Param |Action                     |
|-----|-----------|---------------------------|
|-h   |--help     |Show usage help            |
|-v   |--version  |Show version               |
|-c   |--config   |Specify configuration file (default is rpismingharness.conf)|
|-V   |--verbose  |Output debug info          |
|-R   |--reset    |Reset the ESP              |
|-r   |--run      |Start ESP, boot from flash |
|-f   |--flash    |Start ESP in flash mode    |
|-p   |--powerdown|Put ESP in power down mode |
|-t   |--test     |Specify the test to run (default is all tests)|

Without any command line parameters, rpiSmingHarness will run through all unit tests. (_Not yet implemented!_)
