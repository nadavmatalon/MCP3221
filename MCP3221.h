/*==============================================================================================================*

    @file     MCP3221.h
    @author   Nadav Matalon
    @license  MIT (c) 2016 Nadav Matalon

    MCP3221 Driver (12-BIT Single Channel ADC with I2C Interface)

    Ver. 1.0.0 - First release (16.10.16)

 *===============================================================================================================*
    INTRODUCTION
 *===============================================================================================================*

    The MCP3221 is a 12-Bit Single Channel ADC with hardware I2C interface.

    This library contains a driver for the MCP3221 allowing the user to get raw conversion data or voltage 
    readings based on that data, rolling average of these reading/voltages,  as well as the ability to set the 
    voltage reference (0V-5V) and upper limit of voltage measurment (0V-12V).

 *===============================================================================================================*
    I2C ADDRESSES
 *===============================================================================================================*

    Each MCP3221 has 1 of 8 possible I2C addresses (factory hardwired & recognized by its specific
    part number & top marking on the package itself):

         PART              DEVICE I2C ADDRESS         PART
        NUMBER         (BIN)      (HEX)     (DEC)    MARKING
    MCP3221A0T-E/OT   01001000    0x48       72        GE
    MCP3221A1T-E/OT   01001001    0x49       73        GH
    MCP3221A2T-E/OT   01001010    0x4A       74        GB
    MCP3221A3T-E/OT   01001000    0x4B       75        GC
    MCP3221A4T-E/OT   01001100    0x4C       76        GD
    MCP3221A5T-E/OT   01001101    0x4D       77        GA
    MCP3221A6T-E/OT   01001110    0x4E       78        GF
    MCP3221A7T-E/OT   01001111    0x4F       79        GG

*===============================================================================================================*
   VOLTAGE DIVIDER INPUT (OPTIONAL)
 *===============================================================================================================*

   // ADD DIAGRAM
 
 *===============================================================================================================*
    LICENSE
 *===============================================================================================================*

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

 *==============================================================================================================*/

#if 1
__asm volatile ("nop");
#endif

#ifndef MCP3221_h
#define MCP3221_h

#if !defined(ARDUINO_ARCH_AVR)
#error “The MCP3221 library only supports AVR processors.”
#endif

#include <Arduino.h>
#include "WSWire.h"
#include "utility/MCP3221_PString.h"

namespace Mcp3221 {
    
    const int          DATA_BYTES       =     2;     // number of data bytes requested from the device
    const byte         MIN_CON_TIME     =    15;     // single conversion time with a small overhead (in uS)
    const byte         COM_SUCCESS      =     0;     // I2C communication success Code (No Error)
    const unsigned int MIN_VREF         =  2700;     // minimum Voltage Reference value in mV (same as VCC)
    const unsigned int MAX_VREF         =  5500;     // minimum Voltage Reference value in mV (same as VCC)
    const int          DEFAULT_VREF     =  4096;     // default Voltage Reference value in mV (same as VCC)
    const unsigned int MIN_ALPHA        =     1;     // minimum value of alpha (slowest change) (for EMAVG)
    const unsigned int MAX_ALPHA        =   256;     // maximum value of alpha (raw change/no filter) (for EMAVG)
    const unsigned int DEFAULT_ALPHA    =   178;     // default value of alpha (for EMAVG)
    const byte         MIN_NUM_SAMPLES  =     1;     // minimum number of samples (for Rolling-Average smoothing)
    const byte         MAX_NUM_SAMPLES  =    20;     // maximum number of samples (for Rolling-Average smoothing)

    typedef enum:byte {
        NONE        = 0,
        ROLLING_AVG = 1,
        EMAVG       = 2   // Default
    } smoothing_t;
    
    class MCP3221 {
        public:
            MCP3221(int devAddr);
            ~MCP3221();
            byte ping();
            unsigned int getVref();
            unsigned int getRes1();
            unsigned int getRes2();
            unsigned int getAlpha();
            byte         getNumSamples();
            byte         getSmoothingMethod();
            unsigned int getData();
//            unsigned int getVoltage();
            byte         getComResult();
            void         setVref(unsigned int newVref);
            void         setRes1(unsigned int newRes1);
            void         setRes2(unsigned int newRes2);
            void         setAlpha(unsigned int newAlpha);
            void         setNumSamples(byte newNumSamples);
            void         setSmoothingMethod(smoothing_t newSmoothingMethod);
        private:
            int          _devAddr;
            unsigned int _vRef;
            unsigned int _res1;
            unsigned int _res2;
            unsigned int _alpha;
            byte         _smoothing;        // change byte to smoothing_t?
            byte         _numSamples;
            unsigned int _samples[];
            byte         _comBuffer;
//            unsigned int getRollingAVG();
//            unsigned int updateRollingAVG();
//            unsigned int getEMAVG();
            friend       MCP3221_PString MCP3221ComStr(const MCP3221&);
            friend       MCP3221_PString MCP3221InfoStr(const MCP3221&);
    };
}

using namespace Mcp3221;

#endif
