
# ADS1110 AVR DRIVER LIBRARY

The __ADS1110__ is a 16-Bit Single-Channel (Single-Ended or Differential) ADC IC with Onboard Reference (2.048V), PGA & HW I2C capabilities
in a SOT-23-6 package.

This library contains a driver for the ADS1110 offering complete control over all its Configuration Settings and giving the user the 
ability to rcieve either raw data, voltage reading or percentage reading.


## Repository Contents

* **ADS1110.h** - Header file of the library.
* **ADS1110.cpp** - Compilation file of the library.
* **/examples** - Contains an example sketch for testing all Configuration Settings & Modes of Operation controllable by the library.
* **/extras** - Complementary documentation (End-User License, etc.)
* **keywords.txt** - Keywords for this library which will be highlighted in sketches within the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager.
* **README.md** - The readme file for this library.
* **library.json** - JSON file for the Arduino package manager.


## HOOK-UP

* __PIN 1__ - Connect __V+__ to voltage source to be measured.
* __PIN 2__ - Connect __GND__ to Arduino GND.
* __PIN 3__ - Connect __SCL__ to Arduino PIN A5 with a 2K2 pull-up resistor.
* __PIN 4__ - Conntect __SDA__ to Arduino PIN A4 with a 2K2 pull-up resistor.
* __PIN 5__ - Connect __VCC__ to Arduino 5V output.
* __PIN 6__ - Connect __V-__ either to: (1) Arduino GND (for Single-Ended voltage readings ranging 0-2.048V); or to (2) an external voltage reference of 2.048V (for Single-Ended voltage readings ranging 0-4.096V); or to (3) A 'negative' input (for differential voltage readings between positive and 'negative' inputs).

>__Note__: The 'negative' input is put in brackets here because it isn't a 'real' negative voltage - 
i.e. with relation to the circuit's common ground - only with relation to the positive input side!

* __DECOUPING__: Connect a 0.1uF Ceramic Capacitor between VCC & GND PINS.


## I2C COMMUNICATION

>__INPORTANT__: This library uses the '[WSWire](https://github.com/steamfire/WSWireLib/tree/master/Library/WSWire)' library for I2C communication 
between the contoller IC (Master) and thethe ADS1110 (Slave), so it is NECESSARY to have it installed prior to using the current libraty. 
>
>Alternatively, if you wish to use the '[Wire](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/Wire)' - or any other I2C library for that matter - simply change the following line the the 'ADS1110.h' file:
```
#include <WSWire.h>
```
> to this:
```
#include <Wire.h>  // or to whatever I2C library name you are using.
```

> As noted above, whichever library you intend to use for this purpose __must be alredy installed__ for the ADS1110 library to work.


## I2C ADDRESSES

Each ADS1110 has 1 of 8 possible I2C addresses (factory hardwired & recognized by its specific part number & top marking 
on the package itself):

| PART NO.  | BIN     | HEX  | DEC | MARKING |
|-----------|---------|------|-----|---------|
| ADS1110A0 | 1001000 | 0x48 | 72  | ED0     |
| ADS1110A1 | 1001001 | 0x49 | 73  | ED1     |
| ADS1110A2 | 1001010 | 0x4A | 74  | ED2     |
| ADS1110A3 | 1001011 | 0x4B | 75  | ED3     |
| ADS1110A4 | 1001100 | 0x4C | 76  | ED4     |
| ADS1110A5 | 1001101 | 0x4D | 77  | ED5     |
| ADS1110A6 | 1001110 | 0x4E | 78  | ED6     |
| ADS1110A7 | 1001111 | 0x4F | 79  | ED7     |


## LIBRARY INSTALLATION & SETUP

Begin by installing the library either by using the Arduino IDE's Installation Wizard (Arduino Version >1.5) or simply download the library's ZIP folder from GITHUB, extract it, and copy the extraxcted folder to your Arduino 'libraries' folder.

Next, include the library at the top of the sketch as follows:

```
#include <ADS1110.h>
```

At this point you can construct a new ADS1110 object use the following line (at the top of the sketch after the 'include' line):

```
ADS1110 device_name(device_address);
```

>__NOTE__: replace the '__device_name__' above with a name of your choice. Also, make sure to replace the variable '__device_address__' with 
the specific I2C address of your device - see I2C ADDRESSES section above.


## LIBRARY FUNCTIONS

With the library installed & included in the sketch, and an ADS1110 object initiallized, the following functions are available 
(see the sketch itself for actual examples):

__ping();__                                  
Parameters: None  
Description: Searches for the ADS1110 at the defined I2C Bus address  
Returns: Byte containing the relevant success/error code as follows:  

0 ... Success (no error)  
1 ... Buffer overflow  
2 ... Address sent, NACK received  
3 ... Data send, NACK received  
4 ... Other error (lost bus arbitration, bus error, etc.)  
5 ... Timed-out while trying to become Bus Master  
6 ... Timed-out while waiting for data to be sent

__configInfo();__  
Parameters: None.  
Description: Returns a printable String with the device's I2C address & current Configuration Settings (Gain, Sample Rate & Mode)  
Returns: String

__setGain();__  
Parameters: GAIN_1 / GAIN_2 / GAIN_4 / GAIN_8  
Description: Sets the Gain of the ADS1110 (i.e. 1 / 2 / 4 / 8)  
Default: GAIN_1  
Returns: Byte containing the relevant success/error code (see list above)

__setRate();__  
Parameters: SPS_15 / SPS_30 / SPS_60 / SPS_240  
Description: Sets the Sample Rate of the ADS1110 (i.e. 15 / 30 / 60 / 240 Samples per Second)  
Default: 15_SPS  
Returns: Byte containing the relevant success/error code (see list above)

__setMode ();__  
Parameters: CONTINUOUS / SINGLE_SHOT  
Description: Sets the device's Mode of Operation (i.e. Continuous Conversions/Single Conversion)  
Default: CONTINUOUS  
Returns: Byte containing the relevant success/error code (see list above)

__reset();__  
Parameters: None  
Description: Resets the ADS1110 to its default Configuration Settings (namely: GAIN_1, 15_SPS, CONTINUOUS)  
Returns: Byte containing the relevant success/error code (see list above)

__singleCon();__  
Parameters: None  
Conditions: Works only in __SINGLE CONVERSION__ mode  
Description: Obtains the result of a single conversion  
Returns: Int with actual value if successful or -1 if error occured  

__readData();__  
Parameters: None  
Description: Obtains the latest conversion result from the ADS1110  
Returns: Int witg actual value if successful or -1 if error occured   

__readVoltage();__  
Parameters: None  
Returns: Obtains the latest conversion result translated into Volts  
Returns: Float with actual value if successful or -1 if error occured    

__readPercentage();__  
Parameters: None  
Description: Obtains the latest conversion result translated into Percentage (0-100%)  
Returns: Int with actual value if successful or -1 if error occured 


And, lastly, if for whatever reason you wish to destruct an existing ADS1110 object, you can use the following line to do so:

```
~ADS1110 device_name();
```


## RUNNING THE EXAMPLE SKETCH

1) Hook-up the ADS1110 to the Arduino as explained above.
2) If you like, connect a 10K potentiometer to the ADS1110 V+ PIN (potentimeter's first pin goes to GND, 
middle pin to V+, and third pin to 5V).
3) Upload the sketch to the Arduino.
4) Open the Serial Communications Window (make sure the baud-rate is set to 9600).
5) You should be able to see detailed feedback from running each of the possible functions of the library 
(when you get to the part where readings are carried out, play with the potentiomer to check out changes 
in the readings based on the input voltage).


## BUG REPORTS

Please report any issues/bugs/suggestions at the Github Repository of this library [here](https://github.com/nadavmatalon/ADS1110).


## LICENSE

This library is a free software; you can redistribute it and/or modify it under the terms of the 
GNU General Public License as published by the Free Software Foundation; either version 3.0 of 
the License, or any later version.This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
PARTICULAR PURPOSE. See the GNU General Public License for more details.


