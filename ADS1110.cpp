/*======================================================================
 @file     ADS1110.cpp
 @author   Nadav Matalon
 @license  _________
 
 Driver for ADS1110 (Single Channel 16-BIT ADC)
 
 Rev 1.0 - First release (28.3.16)
 
 This library is a free software; you can redistribute it and/or modify it under 
 the terms of the GNU General Public License as published by the Free Software
 Foundation; either version 3.0 of the License, or any later version.
 This program is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
=======================================================================*/

#include <ADS1110.h>

/*=====================================================================*/
// CONSTRUCTOR
/*=====================================================================*/

ADS1110::ADS1110(byte address) {
    ads1110address = address;
}

/*=====================================================================*/
// DESTRUCTOR
/*=====================================================================*/

ADS1110::~ADS1110() {}

/*=====================================================================*/
// PING
/*=====================================================================*/

byte ADS1110::ping() {
    ADS1110::initCall();
    return ADS1110::endCall();
}

/*=====================================================================*/
// GET CONFIGURATION SETTINGS
/*=====================================================================*/

byte ADS1110::config() {
    if (ADS1110::dataRequest() == ADS1110_NUM_OF_BYTES) {
        for (byte i=2; i>0; i--) Wire.read();
        return Wire.read();
    } else {
        return ADS1110::transErr();
    }
}

/*=====================================================================*/
// SET GAIN (GAIN_1 / GAIN_2 / GAIN_4 / GAIN_8)
/*=====================================================================*/

byte ADS1110::setGain(gain_t n_gain) {
    byte n_config = ADS1110::config();
    for (byte i=2; i>0; i--) bitWrite(n_config, i-1, bitRead(n_gain, i-1));
    return ADS1110::updateConfig(n_config);
}

/*=====================================================================*/
// SET SAMPLE RATE (SPS_15 / SPS_30 / SPS_60 / SPS_240)
/*=====================================================================*/

byte ADS1110::setRate(rate_t n_rate) {
    byte n_config = ADS1110::config();
    for (byte i=4; i>2; i--) bitWrite(n_config, i-1, bitRead(n_rate, i-1));
    return ADS1110::updateConfig(n_config);
}

/*=====================================================================*/
// SET MODE OF OPERATION (CONTINUOUS / SINGLE_SHOT)
/*=====================================================================*/

byte ADS1110::setMode(mode_t n_mode) {
    byte n_config = ADS1110::config();
    bitWrite(n_config, 4, bitRead(n_mode, 4));
    return ADS1110::updateConfig(n_config);
}

/*=====================================================================*/
// UPDATE CONFIGURATION SETTINGS
/*=====================================================================*/

byte ADS1110::updateConfig(byte n_config) {
    ADS1110::initCall();
    Wire.write(n_config + 128);
    return ADS1110::endCall();
}

/*=====================================================================*/
// RESET
/*=====================================================================*/

byte ADS1110::reset() {
    ADS1110::initCall();
    Wire.write(ADS1110_DEFULT_SETTINGS);
    return ADS1110::endCall();
}

/*=====================================================================*/
// INITIATE CALL
/*=====================================================================*/

void ADS1110::initCall() {
    Wire.beginTransmission((int)ADS1110::ads1110address);
}

/*=====================================================================*/
// END CALL
/*=====================================================================*/

byte ADS1110::endCall() {
    return Wire.endTransmission();
}
    
/*=====================================================================*/
// DATA REQUEST
/*=====================================================================*/

byte ADS1110::dataRequest() {
    Wire.requestFrom((int)ADS1110::ads1110address, (int)ADS1110_NUM_OF_BYTES);
    return Wire.available();
}

/*=====================================================================*/
// READ RAW DATA
/*=====================================================================*/

int ADS1110::readData() {
    union Data { byte b[2]; int i; } data;
    if (ADS1110::dataRequest() == ADS1110_NUM_OF_BYTES)  {
        for (byte j=2; j>0; j--) data.b[j-1] = Wire.read();
        Wire.read();
    } else {
        data.i = ADS1110::transErr();
    }
    return data.i;
}

/*=====================================================================*/
// INITIATE SINGLE CONVERSION (ONLY IN 'SINGLE-SHOT' MODE)
/*=====================================================================*/

int ADS1110::singleCon() {
    byte n_config = ADS1110::config();
    ADS1110::updateConfig(n_config);
    delay(ADS1110_CONVERSION_TIME);
    return ADS1110::readData();
}

/*=====================================================================*/
// I2C COMMUNICATION ERROR
/*=====================================================================*/

byte ADS1110::transErr() {
    while(Wire.available()) Wire.read();
    return ADS1110_TRANSMITION_ERROR;
}

/*=====================================================================*/
// CONFIGURATION INFO
/*=====================================================================*/

String ADS1110::configInfo(void) {
    byte d_config = ADS1110::config();
    byte d_gain, d_rate;
    String s_address, s_gain, s_rate, s_mode;
    s_address = "\nI2C Address: " + (String)ADS1110::ads1110address;
    d_gain = (d_config & ADS1110_REG_PGA);
    s_gain = "\nGain: " + (String)(1 << d_gain);
    d_rate = (d_config & ADS1110_REG_DR) + 128;
    switch (d_rate) {
        case (ADS1110_REG_DR_15SPS) : s_rate = "\nRate: 15 SPS" ; break;
        case (ADS1110_REG_DR_30SPS) : s_rate = "\nRate: 30 SPS" ; break;
        case (ADS1110_REG_DR_60SPS) : s_rate = "\nRate: 60 SPS" ; break;
        case (ADS1110_REG_DR_240SPS): s_rate = "\nRate: 240 SPS"; break;
    }
    bitRead(d_config, 4) ? s_mode = "\nMode: Single Shot\n" : s_mode = "\nMode: Continuous\n";
    return s_address + s_gain + s_rate + s_mode;
}

/*=====================================================================*/
// READ DATA AS VOLTAGE
/*=====================================================================*/

float ADS1110::readVoltage() {
    int current_reading;
    byte d_config = ADS1110::config();
    byte d_gain = (1 << (d_config & ADS1110_REG_PGA));
    long d_minCode = ADS1110::minCode(d_config + 128);
    if (bitRead(d_config, 4)) {                         // Single-Shot mode
        current_reading = ADS1110::singleCon();
        if (current_reading != -1) return (current_reading * 2.048) / (d_gain * d_minCode);
        else return current_reading;
    } else {                                            // Continuous mode
        current_reading = ADS1110::readData();
        if (current_reading != -1) return (current_reading * 2.048) / (d_gain * d_minCode);
        else return current_reading;
    }
}

/*=====================================================================*/
// READ DATA AS PERCENTAGE
/*=====================================================================*/

int ADS1110::readPercentage() {
    int current_reading;
    byte d_config = ADS1110::config() + 128;
    float upperLimit = ADS1110::minCode(d_config) - 1.0;
    if (bitRead(d_config, 4)) {                         // Single-Shot mode
        current_reading = ADS1110::singleCon();
        if (current_reading != -1) return round(current_reading * 100.0 / upperLimit);
        else return current_reading;
    } else {                                            // Continuous mode
        current_reading = ADS1110::readData();
        if (current_reading != -1) return round(current_reading * 100.0 / upperLimit);
        else return current_reading;
    }
}

/*=====================================================================*/
// GET MINIMAL CODE BASED ON SAMPLE RATE
/*=====================================================================*/

long ADS1110::minCode(byte d_config) {
    switch (d_config & ADS1110_REG_DR) {
        case (ADS1110_REG_DR_15SPS) : return ADS1110_MIN_CODE_SPS15;  break;
        case (ADS1110_REG_DR_30SPS) : return ADS1110_MIN_CODE_SPS30;  break;
        case (ADS1110_REG_DR_60SPS) : return ADS1110_MIN_CODE_SPS60;  break;
        case (ADS1110_REG_DR_240SPS): return ADS1110_MIN_CODE_SPS240; break;
    }
}
