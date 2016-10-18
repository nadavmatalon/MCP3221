
# MCP3221 AVR DRIVER LIBRARY

The __MCP3221__ is a 12-Bit ADC IC with a hardware I2C interface in a SOT-23-5 package.

This library contains a full driver for the MCP3221 exposing all its available features.

## Repository Contents

UPDATE LIST OF FILES

* **MCP3221.h** - Header file of the library.
* **MCP3221.cpp** - Compilation file of the library.
* **/examples** - Contains an example sketch for testing all Configuration Settings & Modes of Operation controllable by the library.
* **/extras** - Complementary documentation (End-User License, etc.)
* **keywords.txt** - Keywords for this library which will be highlighted in sketches within the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager.
* **README.md** - The readme file for this library.
* **library.json** - JSON file for the Arduino package manager.


## HOOK-UP

* __PIN 1 (VCC/VREF)__ - Serves as both Power Supply input and Voltage Reference for the ADC. Connect 5V output of Arduino's' or other equivalent power source (5.5V max). However, if using external power source, remember to connect all GND together.
* __PIN 2 (GND)__ - connect to Arduino GND
* __PIN 3 (VIN)__ - Connect input voltage to be measured (RANGE: GND - VCC, Min: 0V, Max: 5.5V). For higher voltage readings (eg. Min: 8.444V, Max: 12.881V), use a voltage divider (Resistor 1: 10K; Resisor 2: 4K7)
* __PIN 4 (SDA)__ - Connect to Arduino's PIN A4 with a 2K2 (400MHz I2C Bus speed) or 10K (100MHz I2C Bus speed) pull-up resistor
* __PIN 5 (SCL)__ - Connect to Arduino's PIN A5 with a 2K2 (400MHz I2C Bus speed) or 10K (100MHz I2C Bus speed) pull-up resistor
* __DECOUPING__: Minimal decoupling consists of a 0.1uF Ceramic Capacitor between the VCC & GND PINS. For improved performance, add a 1uF and a 10uF Ceramic Capacitors as well across these pins.

## GENERAL NOTES

1) __I2C Communications Library__

This library uses the '[WSWire](https://github.com/steamfire/WSWireLib/tree/master/Library/WSWire)' library for I2C communication between the contoller IC (Master) and the MCP9802 (Slave), so it is __NECESSARY__ to have it installed prior to using the current libraty. 

Alternatively, if you wish to use the '[Wire](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/Wire)' - or any other I2C library for that matter - simply change the following line the the __MCP9802.h__ file:
```
#include <WSWire.h>
```
to this:
```
#include <Wire.h>  // or to whatever I2C library name you are using.
```
As noted above, whichever library you intend to use for this purpose __must be alredy installed__ for the MCP9802 library to work.







## I2C ADDRESSES

Each MCP3221 has 1 of 8 possible I2C addresses (factory hardwired & recognized by its specific part number & top marking 
on the package itself):

|PART NO.         | BIN      | HEX  | DEC | MARKING |
|-----------------|----------|------|-----|---------|
||MCP3221A0T-E/OT | 01001000 | 0x48 | 72  | GE      |
| MCP3221A1T-E/OT | 01001001 | 0x49 | 73  | GH      |
| MCP3221A2T-E/OT | 01001010 | 0x4A | 74  | GB      |
| MCP3221A3T-E/OT | 01001000 | 0x4B | 75  | GC      |
| MCP3221A4T-E/OT | 01001100 | 0x4C | 76  | GD      |
| MCP3221A5T-E/OT | 01001101 | 0x4D | 77  | GA      |
| MCP3221A6T-E/OT | 01001110 | 0x4E | 78  | GF      |
| MCP3221A7T-E/OT | 01001111 | 0x4F | 79  | GG      |


## LIBRARY INSTALLATION & SETUP

Begin by installing the library either by using the Arduino IDE's Installation Wizard (Arduino Version >1.5) or simply download the library's ZIP folder from GITHUB, extract it, and copy the extraxcted folder to your Arduino 'libraries' folder.

Next, include the library at the top of the sketch as follows:

```
#include <MCP3221.h>
```

At this point you can construct a new MPC3221 object(s) by using the following command (at the top of the sketch after the 'include' line):

```
MCP3221 device_name(device_address);
```

>__NOTE__: replace the '__device_name__' above with a name of your choice. Also, make sure to replace the variable '__device_address__' with the specific I2C address of your device - see I2C ADDRESSES section above.


## LIBRARY FUNCTIONS

With the library installed & included in the sketch, and an MCP9802 object initiallized, the following functions are available (see the usage example sketch for a detailed implementation):

__Note About Methods' Return Values:__  
All 'get' methods return some sort of value (e.g. temp reading, hysteresis setting, etc.), while all 'set' methods return nothing. Nevertheless, ALL methods implicitly update the library's __I2C _comBuffer__ (=communication buffer) after each I2C transmission. The reason for this functional design is to maintain structural coherance between the 'get' and 'set' methods. As 'get' methods cannot return both the desired value and the I2C transmission's result simultaniously. Consequently, if the relevant value hasn't been obtained by a particular 'get' method, the user can simply check the content of the _comBuffer to see what error occured. Similarly, it is possible to check if a particular setting has been successfully applied via a 'set' method either by preforming the corresponding 'get' method - e.g. getHystC() after using setHystC() - or by checking the content of the _comBuffer (0 indicates a successful transmission, 1-6 indicate an error as listed below). 

__ping();__                                  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Searches for the MCP9802 at the pre-defined I2C Bus address &amp; returns byte with the relevant success/error code, as follows:  
0  ... Success (no error)  
1  ... Buffer overflow  
2  ... Address sent, NACK received  
3  ... Data send, NACK received  
4  ... Other error (lost bus arbitration, bus error, etc.)  
5  ... Timed-out while trying to become Bus Master  
6  ... Timed-out while waiting for data to be sent
>6 ... Unlisted error (potential future implementation/s)<br>

Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte  





## RUNNING THE EXAMPLE SKETCH

1) Hook-up the MCP3221 to the Arduino as explained above.
2) Upload the Example Sketch to the Arduino.
3) Open the Serial Communications Window (make sure the baud-rate is set to 9600).
4) You should be able to see detailed feedback from running each of the public methods of the library. 


## BUG REPORTS

Please report any issues/bugs/suggestions at the 'Issues' section of this Github repository.

## TODO

## VERSION HISTORY

__Ver. 1.0.0__ - First release (16.10.16)  

## LICENSE

[The MIT License (MIT)](https://opensource.org/licenses/MIT)
Copyright (c) 2016 Nadav Matalon

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


