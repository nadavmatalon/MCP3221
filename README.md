
# MCP3221 AVR DRIVER LIBRARY

The __MCP3221__ is a 12-Bit ADC IC with a hardware I2C interface in a SOT-23-5 package.

This library contains a full driver for the MCP3221 exposing all its available features.

## Repository Contents

* **MCP3221.h** - Header file of the library.
* **MCP3221.cpp** - Compilation file of the library.
* **/examples** - Contains an example sketch for testing all Configuration Settings & Modes of Operation controllable by the library.
* **/extras** - Complementary documentation (End-User License, etc.)
* **keywords.txt** - Keywords for this library which will be highlighted in sketches within the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager.
* **README.md** - The readme file for this library.
* **library.json** - JSON file for the Arduino package manager.


## HOOK-UP

* __PIN 1 (VCC/VREF)__ - Serves as both Power Supply input and Voltage Reference for the ADC. Connect to a Power Source (5.5V max) - e.g. 4.096V for exact 1mV per measurement unit or to the Arduino 5V Output pin.
* __PIN 2 (GND)__ - GND
* __PIN 3 (VIN)__ - Connect input voltage to be measured (RANGE: GND - VCC, Min: 0V, Max: 5.5V). For higher voltage readings (Min: 8.444, Max: 12.881), use a voltage divider (Resistor 1: 10K, Resisor 2: 4K7)
* __PIN 4 (SDA)__ - Conntect __SDA__ to Arduino PIN A4 with a 2K2 (400MHz I2C Bus) or 10K (100MHz I2C Bus) pull-up resistor
* __PIN 5 (SCL)__ - Connect to Arduino PIN A5 with a 2K2 (400MHz I2C Bus) or 10K (100MHz I2C Bus) pull-up resistor

* __I2C BUS SPEED__ - For 'Fast' I2C Bus speed (400MHz) use 2K2 pull-up resistors on the SDA & SCL lines. For 'Standard' I2C Bus speed use 10K pull-up resistors on these lines.  

* __DECOUPING__: Minimal decoupling consists of a 0.1uF Ceramic Capacitor between the VCC & GND PINS. For improved performance, add a 1uF and a 10uF Ceramic Capacitors as well across these pins.


## I2C COMMUNICATION

>__INPORTANT__: This library uses the '[WSWire](https://github.com/steamfire/WSWireLib/tree/master/Library/WSWire)' library for I2C communication 
between the contoller IC (Master) and thethe ADS1110 (Slave), so it is NECESSARY to have it installed prior to using the current libraty. 
>
>Alternatively, if you wish to use the '[Wire](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/Wire)' - or any other I2C library for that matter - simply change the following line the the 'MCP9802.h' file:
```
#include <WSWire.h>
```
> to this:
```
#include <Wire.h>  // or to whatever I2C library name you are using.
```

> As noted above, whichever library you intend to use for this purpose __must be alredy installed__ for the MCP9802 library to work.


## I2C ADDRESSES

Each MCP3221 has 1 of 8 possible I2C addresses (factory hardwired & recognized by its specific part number & top marking 
on the package itself):

|PART NO.         | BIN      | HEX  | DEC | MARKING |
|-----------------|----------|------|-----|---------|
||MCP3221A0T-E/OT | 01001000 | 0x48 | 72  | GE      |
| MCP3221A0T-E/OT | 01001001 | 0x49 | 73  | GH      |
| MCP3221A0T-E/OT | 01001010 | 0x4A | 74  | GB      |
| MCP3221A0T-E/OT | 01001000 | 0x4B | 75  | GC      |
| MCP3221A0T-E/OT | 01001100 | 0x4C | 76  | GD      |
| MCP3221A0T-E/OT | 01001101 | 0x4D | 77  | GA      |
| MCP3221A0T-E/OT | 01001110 | 0x4E | 78  | GF      |
| MCP3221A0T-E/OT | 01001111 | 0x4F | 79  | GG      |


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

With the library installed & included in the sketch, and an MCP3221 object initiallized, the following functions are available 
(see the example sketch for a detailed implementation):

__NOTE:__ All 'get' methods return some sort of value, while all 'set' methods return nothing. Nevertheless, ALL methods which use the I2C communication protocol implicitly update the library's I2C _comBuffer (=communication buffer) after each I2C transaction. The reason for this functional setup is that the said 'get' methods cannot return both the desired value from the device (e.g. current conversion data) and the I2C transaction's result simultaniously.  Consequently, if the relevant value hasn't been obtained by a particular 'get' method, simply check the content of the _comBuffer to see which error occured (0 indicates a successful I2C transaction, 1-6 indicate an error as listed below). 

__ping();__                                  
Parameters: None  
Description: Searches for the MCP3221 at the pre-defined I2C Bus address  
Returns: Byte containing the relevant success/error code as follows:  

0 ... Success (no error)  
1 ... Buffer overflow  
2 ... Address sent, NACK received  
3 ... Data send, NACK received  
4 ... Other error (lost bus arbitration, bus error, etc.)  
5 ... Timed-out while trying to become Bus Master  
6 ... Timed-out while waiting for data to be sent

__getTempC16();__  
Parameters: None 
Description: Returns current temperature reading in degrees Celsius times 16
Returns: int

__singleConC16();__  
Parameters: None 
Conditions: Only works in Single-Shot mode
Description: Carries out a single conversion & returns a temperature reading in degrees Celsius times 16
Returns: int

__getHystC16();__  
Parameters: None 
Description: Returns the current Hysteresis register value in degrees Celsius times 16
Returns: int

__setHystC();__  
Parameters: int (range: -55 to +125)
Description: Sets the Hysteresis register value in degrees Celsius
Returns: None



And, lastly, if for whatever reason you wish to destruct an existing MCP9802 object, you can use the following line to do so:

```
~MCP3221 device_name();
```


## RUNNING THE EXAMPLE SKETCH

1) Hook-up the MCP3221 to the Arduino as explained above.
2) Upload the Example Sketch to the Arduino.
3) Open the Serial Communications Window (make sure the baud-rate is set to 9600).
4) You should be able to see detailed feedback from running each of the public methods of the library. 


## BUG REPORTS

Please report any issues/bugs/suggestions at the 'Issues' section of this Github repository.


## LICENSE

This library is a free software; you can redistribute it and/or modify it under the terms of the 
GNU General Public License as published by the Free Software Foundation; either version 3.0 of 
the License, or any later version.This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
PARTICULAR PURPOSE. See the GNU General Public License for more details.


