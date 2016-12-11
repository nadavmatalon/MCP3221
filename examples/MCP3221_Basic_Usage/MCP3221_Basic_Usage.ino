/* 
  MCP3221 LIBRARY - BASIC USAGE EXAMPLE
  -------------------------------------

  INTRODUCTION
  ------------
  This sketch presents a minimal usage example with the MCP3221 Library in which a voltage reading is performed evey 600mS 
  from a 5V input.  

  WIRING DIAGRAM
  --------------
                                       MCP3221
                                       -------
                                VCC --| •     |-- SCL
                                      |       |
                                GND --|       |
                                      |       |
                                AIN --|       |-- SDA
                                       -------

  PIN 1 (VCC/VREF) - Serves as both Power Supply input and Voltage Reference for the ADC. Connect to Arduino 5V output or any other
                equivalent power source (5.5V max). If using an external power source, remember to connect all GND's together
  PIN 2 (GND) - connect to Arduino GND
  PIN 3 (AIN) - Connect to Arduino's 3.3V Output or to the middle pin of a 10K potentiometer (the pot's first pin goes to GND and the third to 5V)
  PIN 4 (SDA) - Connect to Arduino's PIN A4 with a 2K2 (400MHz I2C Bus speed) or 10K (100MHz I2C Bus speed) pull-up resistor
  PIN 5 (SCL) - Connect to Arduino's PIN A5 with a 2K2 (400MHz I2C Bus speed) or 10K (100MHz I2C Bus speed) pull-up resistor
  DECOUPING:    Minimal decoupling consists of a 0.1uF Ceramic Capacitor between the VCC & GND PINS. For improved performance,
                add a 1uF and a 10uF Ceramic Capacitors as well across these pins

  I2C ADDRESSES
  -------------
  Each MCP3221 has 1 of 8 possible I2C addresses (factory hardwired & recognized by its specific part number & top marking
  on the package itself):

       PART                  DEVICE I2C ADDRESS          PART
      NUMBER             (BIN)      (HEX)     (DEC)     MARKING
  MCP3221A0T-E/OT       01001000      0x48       72       GE
  MCP3221A1T-E/OT       01001001      0x49       73       GH
  MCP3221A2T-E/OT       01001010      0x4A       74       GB
  MCP3221A3T-E/OT       01001011      0x4B       75       GC
  MCP3221A4T-E/OT       01001100      0x4C       76       GD
  MCP3221A5T-E/OT       01001101      0x4D       77       GA
  MCP3221A6T-E/OT       01001110      0x4E       78       GF
  MCP3221A7T-E/OT       01001111      0x4F       79       GG

  BUG REPORTS
  -----------
  Please report any bugs/issues/suggestions at the GITHUB Repository of this library at: https://github.com/nadavmatalon/MCP3221

  LICENSE
  -------
  The MIT License (MIT)
  Copyright (c) 2016 Nadav Matalon
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
  documentation files (the "Software"), to deal in the Software without restriction, including without
  limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
  conditions:
  
  The above copyright notice and this permission notice shall be included in all copies or substantial
  portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
  LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "MCP3221.h"

const byte DEV_ADDR = 0x4D;                            // I2C address of the MCP3221 (Change as needed)

unsigned long timeNow;

MCP3221 mcp3221(DEV_ADDR);

void setup() {
    Serial.begin(9600);
    Wire.begin();
    Serial.print(F("\n\nserial is open\n\n"));
    mcp3221.setVref(4096);                            // sets voltage reference for the ADC in mV (change as needed)
    mcp3221.setVinput(VOLTAGE_INPUT_5V);              // sets voltage input type to be measured (change as needed)
    mcp3221.setRes1(10000);                           // sets exact value of the voltage divider's Resistor 1 for 12V readings (change as needed)
    mcp3221.setRes2(4700);                            // sets exact value of the voltage divider's Resistor 2 for 12V readings (change as needed)
    mcp3221.setAlpha(178);                            // sets the Alpha value used by the EMAVG smoothing method (change as needed)
    timeNow = millis();
}

void loop() {
    if (millis() - timeNow >= 600) {
        Serial.print(F("reading:\t"));
        Serial.print(mcp3221.getVoltage());
        Serial.print(F("mV\n\n"));  
        timeNow = millis();
    }
}
