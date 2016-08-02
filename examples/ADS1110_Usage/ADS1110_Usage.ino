/* 
  ADS1110 LIBRARY - COMPLETE USAGE EXAMPLE SKETCH
  -----------------------------------------------

  This sketch exposes all the functions offered by the ADS1110 library as detailed below, thereby offering a quick & easy way of testing your ADS1110.

  INPORTANT: This library uses the 'WSWire' library (https://github.com/steamfire/WSWireLib/tree/master/Library/WSWire) for I2C communication with 
  the ADS1110, so it is NECESSARY to have it installed prior to using the current libraty. Alternatively, if you wish to use the 'Wire' library 
  (https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/Wire) - or any other I2C library for that matter - simply change 
  the following line the the 'ADS1110.h' file:
      #include <WSWire.h>
  to this:
      #include <Wire.h>  // or to whatever I2C library name you are using.
  As noted above, whichever library you intend to use for this purpose must be alredy installed for the ADS1110 library to work.


  WIRING DIAGRAM
  --------------
                                       ADS1110
                                       -------
      (to Positive Input: 0-5V)  V+ --| •     |-- V- (to GND / 2.048V Reference / 'Negative' Input 0-5V)
                                      |       |
                                GND --|       |-- VCC
                                      |       |
                                SCL --|       |-- SDA
                                       -------

   PIN 1 - Connect V+ to voltage source to be measured.
   PIN 2 - Connect GND to Arduino GND.
   PIN 3 - Connect SCL to Arduino PIN A5 with a 2K2 pull-up resistor.
   PIN 4 - Conntect SDA to Arduino PIN A4 with a 2K2 pull-up resistor.
   PIN 5 - Connect VCC to Arduino 5V output.
   PIN 6 - Connect V- either to Arduino GND (for Single-Ended voltage readings ranging 0-2.048V),
           or to an external voltage reference of 2.048V (for Single-Ended voltage readings ranging 0-4.096V),
           or to 'negative' input (for differential voltage readings between positive and 'negative' inputs).
           (Note: the 'negative' input is put in brackets here because it isn't a 'real' negative voltage - 
            i.e. with relation to the circuit's common ground - only with relation to the positive input side!)
   DECOUPING: Connect a 0.1uF Ceramic Capacitor between VCC & GND PINS.

 
  I2C ADDRESSES
  -------------
  Each ADS1110 has 1 of 8 possible I2C addresses (factory hardwired & recognized by its specific part number & top marking 
  on the package itself):
 
    PART               DEVICE I2C ADDRESS          PART
   NUMBER          (BIN)      (HEX)     (DEC)     MARKING
  ADS1110A0       1001000      0x48       72        ED0
  ADS1110A1       1001001      0x49       73        ED1
  ADS1110A2       1001010      0x4A       74        ED2
  ADS1110A3       1001011      0x4B       75        ED3
  ADS1110A4       1001100      0x4C       76        ED4
  ADS1110A5       1001101      0x4D       77        ED5
  ADS1110A6       1001110      0x4E       78        ED6
  ADS1110A7       1001111      0x4F       79        ED7

  LIBRARY INSTALLATION & SETUP
  ----------------------------

  Begin by installing the library either by using the Arduino IDE's installation wizard (Arduino Version >1.5) 
  or simply download the librarty ZIP folder from GITHUB (LINK), extract it, and copy the extraxcted folder 
  to your Arduino 'libraries' folder.
  
  Next, include the library at the top of the sketch as follows:

  #include <ADS1110.h>

  At this point you can construct a new ADS1110 object use the following line (at the top of the sketch after the 'include' line):

  ADS1110 device_name(device_address);

  (NOTES: replace the 'device_name' above with a name of your choice. Also, make sure to replace the variable 'device_address' with 
   the specific I2C address of your device - see I2C ADDRESSES section above).

  
  LIBRARY FUNCTIONS
  -----------------
  With the library installed & included in the sketch, and an ADS1110 object initiallized, the following functions are available 
  (see the sketch itself for actual examples):
  
    FUNCTION (PARAMETERS)                     NOTES
    ----------------------                    -----
    ping();                                   No params. Searches for the device at the defined I2C address and returns 1 (Success) or 0 (Failure).
    configInfo();                             No params. Returns a string with the device's current Configuration Settings (i.e. Gain, Sample Rate & Mode). 
                                              If unsuccessful, the function returns the relevant an Error Code (see I2C Error Codes list below).
    setGain(GAIN_1/GAIN_2/GAIN_4/GAIN_8);     Sets the Gain of the device (i.e. 1/2/4/8, Default: 1) and returns 1 (Success) or 0 (Failure). 
    setRate(SPS_15/SPS_30/SPS_60/SPS_240);    Sets the Sample Rate of the device (i.e. 15/30/60/240 Samples per Second, Default: 15 SPS) and returns 1 (Success) or 0 (Failure).
    setMode (CONTINUOUS/SINGLE_SHOT);         Sets the device's Mode of Operation (i.e. CONTINUOUS/SINGLE SHOT, Default: CONTINUOUS) and returns 1 (Success) or 0 (Failure).
    reset();                                  No params. Resets the device to its default Configuration Settings and returns 1 (Success) or 0 (Failure).
    singleCon();                              No params. In SINGLE CONVERSION mode, this function returns a single byte containing the result of a single conversion.
    readData();                               No params. In CONTINUOUS mode, this function returns a single byte containing the current conversion result from the device.
    readVoltage();                            No params. In CONTINUOUS mode (CHECK??), this function returns a float containing the current conversion result translated into Volts.
    readPercentage();                         No params. In CONTINUOUS mode (CHECK??), this function returns a single byte containing the current conversion result 
                                              translated into Percentage (0-100%).

  And, lastly, if for whatever reason you wish to destruct an existing ADS1110 object, you can use the following line to do so:

    ~ADS1110 device_name();


  I2C ERROR CODES
  ---------------
    0 ... Success (no error)
    1 ... Buffer overflow
    2 ... Address sent, NACK received
    3 ... Data send, NACK received
    4 ... Other error (lost bus arbitration, bus error, etc.)
    5 ... Timed-out while trying to become Bus Master
    6 ... Timed-out while waiting for data to be sent


  RUNNING THE COMPLETE USAGE EXAMPLE SKETCH
  -----------------------------------------
  1) Hook-up the ADS1110 to the Arduino as explained above.
  2) If you like, connect a 10K potentiometer to the ADS1110 V+ PIN (potentimeter's first pin goes to GND, 
     middle pin to V+, and third pin to 5V).
  3) Upload the sketch to the Arduino.
  4) Open the Serial Communications Window (make sure the baud-rate is set to 9600).
  5) You should be able to see detailed feedback from running each of the possible functions of the library 
     (when you get to the part where readings are carried out, play with the potentiomer to check out changes 
     in the readings based on the input voltage).


  BUG REPORTS
  -----------
  Please report any bugs/issues/suggestions at the GITHUB Repository of this library at:  _________________________.


  LICENSE
  -------
  This library is a free software; you can redistribute it and/or modify it under the terms of the 
  GNU General Public License as published by the Free Software Foundation; either version 3.0 of 
  the License, or any later version.This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
  PARTICULAR PURPOSE. See the GNU General Public License for more details.

*/


#include <ADS1110.h>

#define ADS1110_1_ADDRESS  0x48         // Decimal: 72 - Specific I2C address of a given ADC1110 - Change as needed
#define DELAY_TIME         0x3E8        // Decimal: 1000 - Just a quick pause between testing various functions

ADS1110 ADS1110_1(ADS1110_1_ADDRESS);   // Constructs the ADS1110 object with the given address.

String testResult;                      // Contains test feedback in a String format for printing in the Serial Communications Window.

void setup() {
    Serial.begin(9600);                 // Initiallizes the Serial Port (at 9600bd)
    Wire.begin();                       // Initiallizes the I2C communications bus
    while(!Serial);                     // Waits for Serial Port to initialize
    Serial.print("\nSerial Open\n");
    testPing();                         // Pings the ADS1110 to make sure I2C communication is established
    configTests();                      // Runs the various Configuration Settings tests
    dataTests();                        // Runs the various Data aquisition tests
    Serial.print("\nDone Testing\n");
}

void loop() {}

void testPing() {
    testResult = (ADS1110_1.ping() ? "No Response" : "Pong");
    Serial.print("\nPing device: " + testResult + "\n");
    takeAquickBreak();
}

void configTests() {
    testConfigInfo();
    testGain();
    takeAquickBreak();
    testRate();
    takeAquickBreak();
    testMode();
    takeAquickBreak();
    testReset();
}

void dataTests() {
    testReadData();
    takeAquickBreak();
    testReadVolatage();
    takeAquickBreak();
    testReadPercentage();
    takeAquickBreak();
    testInitCon();
}

void testConfigInfo() {
    takeAquickBreak();
    testResult = ADS1110_1.configInfo(); 
    Serial.print("\nConfiguration Info:" + testResult + "\n");
    takeAquickBreak();
}

void testGain() {
    gain_t gains[5] = { GAIN_1, GAIN_2, GAIN_4, GAIN_8, GAIN_1 };
    String gainStr[5] = { "GAIN 1", "GAIN 2", "GAIN 4", "GAIN 8", "GAIN 1" };
    for (byte i=0; i<6; i++) {
        testResult = (ADS1110_1.setGain(gains[i]) ? "Failed" : "Success");
        Serial.print("\nSet Gain to " + gainStr[i] + ": " + testResult + "\n");
        testConfigInfo();
    }
    deviceReset();
}

void testRate() {
    rate_t rates[5] = { SPS_15, SPS_30, SPS_60, SPS_240, SPS_15 };
    String rateStr[5] = { "15 SPS", "30 SPS", "60 SPS", "240 SPS", "15 SPS" };
    for (byte i=0; i<6; i++) {
        testResult = (ADS1110_1.setRate(rates[i]) ? "Failed" : "Success");
        Serial.print("\nSet Sample Rate to " + rateStr[i] + ": " + testResult + "\n");
        testConfigInfo();
    }
    deviceReset();
}

void testMode() {
    mode_t modes[3] = { CONTINUOUS, SINGLE_SHOT, CONTINUOUS };
    String modeStr[3] = { "CONTINUOUS", "SINGLE_SHOT", " CONTINUOUS" };
    for (byte i=0; i<3; i++) {
        testResult = (ADS1110_1.setMode(modes[i]) ? "Failed" : "Success");
        Serial.print("\nSet Mode to " + modeStr[i] + ": " + testResult + "\n");
        testConfigInfo();
    }
    deviceReset();
}

void testReset() {
    takeAquickBreak();
    Serial.println("\nTesting Device Reset:\n");
    testResult = (ADS1110_1.setGain(GAIN_4) ? "Failed" : "Success");
    Serial.print("\nSet Gain to 4: " + testResult + "\n");
    takeAquickBreak();
    testResult = (ADS1110_1.setRate(SPS_240) ? "Failed" : "Success");
    Serial.print("\nSet Sample Rate to 240 SPS: " + testResult + "\n");
    takeAquickBreak();
    testResult = (ADS1110_1.setMode(SINGLE_SHOT) ? "Failed" : "Success");
    Serial.print("\nSet Mode to Single Shot: " + testResult + "\n");
    testConfigInfo();
    deviceReset();
}

void testReadData() {
    rate_t rates[4] = { SPS_15, SPS_30, SPS_60, SPS_240 };
    for (byte i=0; i<6; i++) {
        ADS1110_1.setRate(rates[i]);
        testConfigInfo();
        for (byte i=0; i<20; i++) {
            testResult = ADS1110_1.readData();
            Serial.print("\nRead Data: " + testResult + "\n");
            takeAquickBreak();
        }
    }
    deviceReset();
}

void testReadVolatage() {
    rate_t rates[4] = { SPS_15, SPS_30, SPS_60, SPS_240 };
    for (byte i=0; i<6; i++) {
        ADS1110_1.setRate(rates[i]);
        testConfigInfo();
        for (byte i=0; i<20; i++) {
            testResult = ADS1110_1.readVoltage();
            Serial.print("\nRead Voltage: " + testResult + "V\n");
            takeAquickBreak();
        }
    }
    deviceReset();
}

void testReadPercentage() {
    rate_t rates[4] = { SPS_15, SPS_30, SPS_60, SPS_240 };
    for (byte i=0; i<6; i++) {
        ADS1110_1.setRate(rates[i]);
        testConfigInfo();
        for (byte i=0; i<20; i++) {
             testResult = ADS1110_1.readPercentage();
             Serial.print("\nRead Percentage: " + testResult + "%\n");
             takeAquickBreak();
        }
    }
    deviceReset();
}

void testInitCon() {
    ADS1110_1.setMode(SINGLE_SHOT);
    testConfigInfo();
    for (byte i=0; i<20; i++) {
        testResult = ADS1110_1.singleCon();
        Serial.print("\nSingle Conversion: " + testResult + "\n");
        takeAquickBreak();
    }
    deviceReset();
}

void takeAquickBreak() {
    delay(DELAY_TIME);
}

void deviceReset() {
    takeAquickBreak();
    testResult = (ADS1110_1.reset() ? "Failed" : "Success");
    Serial.print("\nDeviceReset: " + testResult + "\n");
    testConfigInfo();
}
