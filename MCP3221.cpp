/*==============================================================================================================*
 
    @file     MCP3221.cpp
    @author   Nadav Matalon
    @license  MIT (c) 2016 Nadav Matalon

    MCP3221 Driver (12-BIT Single Channel ADC with I2C Interface)

    Ver. 1.0.0 - First release (16.10.16)

 *==============================================================================================================*
    LICENSE
 *==============================================================================================================*
 
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

#include "MCP3221.h"

/*==============================================================================================================*
    CONSTRUCTOR
 *==============================================================================================================*/

MCP3221::MCP3221(
         byte devAddr,
         unsigned int vRef,
         unsigned int res1,
         unsigned int res2,
         unsigned int alpha,
         voltage_input_t voltageInput,
         smoothing_t smoothingMethod,
         byte numSamples) :
         _devAddr(devAddr),
         _vRef(vRef),
         _alpha(alpha),
         _voltageInput(voltageInput),
         _smoothing(smoothingMethod),
         _numSamples(numSamples) {
    _samples[_numSamples] = { 0 };
    if (((res1 != 0) && (res2 != 0)) && (_voltageInput == VOLTAGE_INPUT_12V)) {
        _res1 = res1;
        _res2 = res2;
    } else if (_voltageInput == VOLTAGE_INPUT_5V) {
        _res1 = 0;
        _res2 = 0;
    } else {
        _res1 = DEFAULT_RES_1;
        _res2 = DEFAULT_RES_2;
    }
    _comBuffer = COM_SUCCESS;
}

/*==============================================================================================================*
    DESTRUCTOR
 *==============================================================================================================*/

MCP3221::~MCP3221() {}

/*==============================================================================================================*
    PING (0 = SUCCESS / 1,2,... = I2C ERROR CODE)
 *==============================================================================================================*/

// See meaning of I2C Error Code return values in README

byte MCP3221::ping() {
    Wire.beginTransmission(_devAddr);
    return Wire.endTransmission();
}

/*==============================================================================================================*
    GET VOLTAGE REFERENCE (2700mV - 5500mV)
 *==============================================================================================================*/

unsigned int MCP3221::getVref() {
    return _vRef;
}

/*==============================================================================================================*
    GET VOLTAGE DIVIDER RESISTOR 1 (Ω)
 *==============================================================================================================*/

unsigned int MCP3221::getRes1() {
    return _res1;
}

/*==============================================================================================================*
    GET VOLTAGE DIVIDER RESISTOR 2 (Ω)
 *==============================================================================================================*/

unsigned int MCP3221::getRes2() {
    return _res2;
}

/*==============================================================================================================*
    GET ALPHA (EMAVG ONLY, RANGE: 1 - 256)
 *==============================================================================================================*/

unsigned int MCP3221::getAlpha() {
    return _alpha;
}

/*==============================================================================================================*
    GET NUMBER OF SAMPLES (ROLLING-AVAREGE ONLY, 1-20 SAMPLES)
 *==============================================================================================================*/

byte MCP3221::getNumSamples() {
    return _numSamples;
}

/*==============================================================================================================*
    GET VOLTAGE INPUT (0 = VOLTAGE_INPUT_5V / 1 = VOLTAGE_INPUT_12V)
 *==============================================================================================================*/

byte MCP3221::getVoltageInput() {
    return _voltageInput;
}

/*==============================================================================================================*
    GET SMOOTHING METHOD (0 = NONE / 1 = ROLLING-AVAREGE / 2 = EMAVG)
 *==============================================================================================================*/

byte MCP3221::getSmoothingMethod() {
    return _smoothing;
}

/*==============================================================================================================*
    SET REFERENCE VOLTAGE (2700mV - 5500mV)
 *==============================================================================================================*/

void MCP3221::setVref(unsigned int newVref) {                                  // PARAM RANGE: 2700-5500
    newVref = constrain(newVref, MIN_VREF, MAX_VREF);
    _vRef = newVref;
}

/*==============================================================================================================*
    SET VOLTAGE DIVIDER RESISTOR 1 (Ω)
 *==============================================================================================================*/

void MCP3221::setRes1(unsigned int newRes1) {
    _res1 = newRes1;
}

/*==============================================================================================================*
    SET VOLTAGE DIVIDER RESISTOR 2 (Ω)
 *==============================================================================================================*/

void MCP3221::setRes2(unsigned int newRes2) {
    _res2 = newRes2;
}

/*==============================================================================================================*
    SET ALPHA (EMAVG SMOOTHING METHOD ONLY)
 *==============================================================================================================*/

void MCP3221::setAlpha(unsigned int newAlpha) {                                      // PARAM RANGE: 1-256
    newAlpha = constrain(newAlpha, MIN_ALPHA, MAX_ALPHA);
    _alpha = newAlpha;
}

/*==============================================================================================================*
    SET NUMBER OF SAMPLES (ROLLING-AVAREGE SMOOTHING METHOD ONLY)
 *==============================================================================================================*/

void MCP3221::setNumSamples(byte newNumSamples) {                                    // PARAM RANGE: 1-20
    newNumSamples = constrain(newNumSamples, MIN_NUM_SAMPLES, MAX_NUM_SAMPLES);
    _numSamples = newNumSamples;
    for (byte i=MAX_NUM_SAMPLES; i>0; i--) _samples[i-1] = 0;
}

/*==============================================================================================================*
    SET VOLTAGE INPUT
 *==============================================================================================================*/

void MCP3221::setVoltageInput(voltage_input_t newVoltageInput) { // PARAMS: VOLTAGE_INPUT_5V / VOLTAGE_INPUT_12V
    _voltageInput = newVoltageInput;
}

/*==============================================================================================================*
    SET SMOOTHING METHOD
 *==============================================================================================================*/

void MCP3221::setSmoothingMethod(smoothing_t newSmoothingMethod) {   // PARAMS: NO_SMOOTHING / ROLLING / EMAVG
    _smoothing = newSmoothingMethod;
}

/*==============================================================================================================*
    GET DATA
 *==============================================================================================================*/

//  SINGLE CONVERSION:         B10011010 / 0x9A / 154
//  CONTINUOUS CONVERSION:     B10011011 / 0x9B / 155

unsigned int MCP3221::getData() {
    unsigned int data;
    Wire.requestFrom(_devAddr, DATA_BYTES);
    if (Wire.available() == DATA_BYTES) data = (Wire.read() << 8) | (Wire.read());
    return data;
}

/*==============================================================================================================*
    GET VOLTAGE (Vref 4.096V: 2700 - 4096mV)
 *==============================================================================================================*/

unsigned int MCP3221::getVoltage() {
    if (_smoothing == EMAVG) {
        static unsigned int emAvg = getData();
        delayMicroseconds(MIN_CON_TIME * 2);
        emAvg = (_alpha * (unsigned long)emAvg + (256 - _alpha) * (unsigned long)emAvg) / 256;
        return calcVoltage(emAvg);
    } else if (_smoothing == ROLLING_AVG) {
        if (_samples[0] != 0) return updateRollingAVG();
        else return getRollingAVG();
    } else {
        return calcVoltage(getData());
    }
}

// first time - get rolling average
// rest - update rolling average

/*==============================================================================================================*
    CALCULATE VOLTAGE FROM READING (mV)
 *==============================================================================================================*/

unsigned int MCP3221::calcVoltage(unsigned int readingData) {
    if (_voltageInput == VOLTAGE_INPUT_5V) return round((_vRef / (float)DEFAULT_VREF) * readingData);
    else return ((readingData * (_res1 + _res2)) / _res2);
}

/*==============================================================================================================*
    GET ROLLING AVERAGE OF MULTIPLE SAMPLE READINGS (mV)
 *==============================================================================================================*/

unsigned int MCP3221::getRollingAVG() {
    unsigned long sum = 0;
    for (byte i=_numSamples; i>0; i--) {
        _samples[i] = calcVoltage(getData());
        delayMicroseconds(MIN_CON_TIME);
        sum += _samples[i];
    }
    return round((float)sum / (float)_numSamples);
}

/*==============================================================================================================*
    UPDATE ROLLING AVERAGE (mV)
 *==============================================================================================================*/

unsigned int MCP3221::updateRollingAVG() {
    unsigned int sum = 0;
    for (byte i=_numSamples-2; i>=0; i--) _samples[i+1] = _samples[i];  // drop last reading & rearrange array
    _samples[0] = calcVoltage(getData());                               // add a new sample
    for (byte j=_numSamples; j>0; j--) sum += _samples[j];              // aggregate all samples
    return round(sum / _numSamples);                                    // return average
}

/*==============================================================================================================*
    GET LATEST I2C COMMUNICATION RESULT (0 = OK / 1..7 = ERROR)
 *==============================================================================================================*/

byte MCP3221::getComResult() {
    return _comBuffer;
}
