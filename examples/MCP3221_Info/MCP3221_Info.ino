/* 
  MCP9802 LIBRARY - DEVICE INFORMATION STRING EXAMPLE
  ---------------------------------------------------

  INTRODUCTION
  ------------
  This sketch presents a minimal example of extending the MCP9802 Library to include an additional function for generating a printable device 
  information string which may be useful, for example, during debugging sessions.

  As can be seen in the sketch below, implementation of this extended functionality only requires adding a single 'include' to the code, namely: 
  to that of the relevant *.h file (MCP9802InfoStr.h).
  
  Note that this functional extension does come at the cost of an increased memory usage, and therefore it seemed preferable to maintain it 
  as an optional add-on rather than include it in the core MCP9802 Library itself.
  
  WIRING DIAGRAM
  --------------
                                       MCP9802
                                       -------
                                VCC --| •     |-- SDA
                                      |       |
                                GND --|       |
                                      |       |
                              ALERT --|       |-- SCL
                                       -------

   PIN 1 -    Connect VCC to the Arduino's 5V output
   PIN 2 -    Connect GND to the Arduino's GND
   PIN 3 -    This pin can be left unconnected for the purpose of running this sketch.
   PIN 4 -    Conntect SCL to the Arduino's PIN A5 with a 2K2 pull-up resistor
   PIN 5 -    Conntect SDA to the Arduino's PIN A4 with a 2K2 pull-up resistor
   DECOUPING: Connect a 0.1uF Ceramic Capacitor between the MCP9802's VCC & GND PINS

  I2C ADDRESSES
  -------------
  Each MCP9802 has 1 of 8 possible I2C addresses (factory hardwired & recognized by its specific part number & top marking 
  on the package itself):
 
    PART               DEVICE I2C ADDRESS          PART
   NUMBER          (BIN)      (HEX)     (DEC)     MARKING
  MCP9802A0       1001000      0x48       72       JKNN
  MCP9802A1       1001001      0x49       73       JLNN
  MCP9802A2       1001010      0x4A       74       JMNN
  MCP9802A3       1001011      0x4B       75       JPNN
  MCP9802A4       1001100      0x4C       76       JQNN
  MCP9802A5       1001101      0x4D       77       JRNN
  MCP9802A6       1001110      0x4E       78       JSNN
  MCP9802A7       1001111      0x4F       79       JTNN

  BUG REPORTS
  -----------
  Please report any bugs/issues/suggestions at the GITHUB Repository of this library at: https://github.com/nadavmatalon/MCP9802

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
#include "utility/MCP3221InfoStr.h"

const byte DEV_ADDR = 0x4D;                       // I2C address of the MCP9802 (Change as needed)

MCP3221 mcp3221(DEV_ADDR);

void setup() {
    Serial.begin(9600);
    Wire.begin();
    while(!Serial);
    Serial.print(F("\n\nCURRENT SETTINGS:\n"));
    Serial.print(MCP3221InfoStr(mcp3221));
    Serial.print(F("\nCHANGING TO NEW SETTINGS..."));
    mcp3221.setVref(5112);
    mcp3221.setSmoothingMethod(ROLLING_AVG);
    mcp3221.setRes1(10251);
    mcp3221.setRes2(4705);
    mcp3221.setVoltageInput(VOLTAGE_INPUT_12V);
    mcp3221.setAlpha(134);
    mcp3221.setNumSamples(16);
    Serial.print(F("DONE\n"));
    Serial.print(MCP3221InfoStr(mcp3221));
    Serial.print(F("\nRESETTING DEVICE..."));
    mcp3221.reset();
    Serial.print(F("DONE\n"));
    Serial.print(MCP3221InfoStr(mcp3221));
    Serial.print(F("\n\n"));
}

// check size of buffer and modify
// change function to 'setSmoothing'
// change method to 'setVinput
// add getPercent function (?)

void loop() {}
