/*=======================================================================

 @file     ADS1110.h
 @author   Nadav Matalon
 @license  _________
 
 Driver for ADS1110 (Single Channel 16-BIT ADC)
 
 Rev 1.0 - First release (28.3.16)

 
 Each ADS1110 has 1 of 8 possible I2C addresses (factory hardwired & recognized by its specific
 part number & top marking on the package itself):
 
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

 This library is a free software; you can redistribute it and/or modify it under 
 the terms of the GNU General Public License as published by the Free Software
 Foundation; either version 3.0 of the License, or any later version.
 This program is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 =======================================================================*/


#ifndef ADS1110_h
#define ADS1110_h

#if defined(ARDUINO_ARCH_AVR)
    #include <Arduino.h>
    #include <WSWire.h>
#else
    #error “The ADS1110 library only supports AVR processors.”
#endif


/*=====================================================================*/
// CONSTANTS
/*=====================================================================*/

#define ADS1110_NUM_OF_BYTES        0x03
#define ADS1110_TRANSMITION_ERROR   -1
#define ADS1110_DEFULT_SETTINGS     0x8C
#define ADS1110_CONVERSION_TIME     0x46   // 70mS

/*=========================================================================================================*/
// MINIMUM DATA VALUES (BASED ON SAMPLE RATE)
/*=========================================================================================================*/

#define ADS1110_MIN_CODE_SPS15    0x8000   // -1 * Minimum data value with 15 Samples per Second Rate (16-Bit)
#define ADS1110_MIN_CODE_SPS30    0x4000   // -1 * Minimum data value with 15 Samples per Second Rate (15-Bit)
#define ADS1110_MIN_CODE_SPS60    0x2000   // -1 * Minimum data value with 15 Samples per Second Rate (14-Bit)
#define ADS1110_MIN_CODE_SPS240    0x800   // -1 * Minimum data value with 15 Samples per Second Rate (12-Bit)

/*=========================================================================================================*/
// CONFIGURATION REGISTER
/*=========================================================================================================*/

#define ADS1110_REG_PGA             0x83    // Config Register Bits 1(PGA1) & 0(PGA0)   B10000011
#define ADS1110_REG_PGA_1           0x80    // Gain 1                                   B10000000 (Default)
#define ADS1110_REG_PGA_2           0x81    // Gain 2                                   B10000001
#define ADS1110_REG_PGA_4           0x82    // Gain 4                                   B10000010
#define ADS1110_REG_PGA_8           0x83    // Gain 8                                   B10000011

#define ADS1110_REG_DR              0x8C    // Config Register Bits 3(DR1) & 2(DR0)     B10001100
#define ADS1110_REG_DR_240SPS       0x80    // 240 Samples per Second                   B10000000
#define ADS1110_REG_DR_60SPS        0x84    //  60 Samples per Second                   B10000100
#define ADS1110_REG_DR_30SPS        0x88    //  30 Samples per Second                   B10001000
#define ADS1110_REG_DR_15SPS        0x8C    //  15 Samples per Second                   B10001100 (Default)

#define ADS1110_REG_MODE            0x90    // Config Register Bit 4(SC)                B10010000
#define ADS1110_REG_MODE_SINGLE     0x90    // Single Shot Conversion Mode              B10010000
#define ADS1110_REG_MODE_CONT       0x80    // Continuous Conversion Mode               B10000000 (Default)

// CONFIG REG BIT 7: READING (DATA STATUS)
#define ADS1110_REG_DATA            0x80    // Config Register Bit 7(ST)                B10000000
#define ADS1110_REG_DATA_NEW        0x00    // New data ready to be read                B00000000
#define ADS1110_REG_DATA_OLD        0x80    // Currently no new data is avialable       B10000000 (Default)

// CONFIG REG BIT 7: WRITING (START CONVERSION)
#define ADS1110_REG_STATE           0x80    // Config Register Bit 7(DRDY)              B10000000
#define ADS1110_REG_SC              0x80    // Start Conversion (Single Shot Mode)      B10000000

/*=========================================================================================================*/

typedef enum {
    GAIN_1 = ADS1110_REG_PGA_1,
    GAIN_2 = ADS1110_REG_PGA_2,
    GAIN_4 = ADS1110_REG_PGA_4,
    GAIN_8 = ADS1110_REG_PGA_8
} gain_t;

typedef enum {
    SPS_15  = ADS1110_REG_DR_15SPS,
    SPS_30  = ADS1110_REG_DR_30SPS,
    SPS_60  = ADS1110_REG_DR_60SPS,
    SPS_240 = ADS1110_REG_DR_240SPS
} rate_t;

typedef enum {
    CONTINUOUS  = ADS1110_REG_MODE_CONT,
    SINGLE_SHOT = ADS1110_REG_MODE_SINGLE
} mode_t;

typedef enum {
    NEW_DATA =  ADS1110_REG_DATA_NEW,
    OLD_DATA =  ADS1110_REG_DATA_OLD,
    START_CONVERSION = ADS1110_REG_SC
} data_t;

class ADS1110 {
    protected:
        byte   ads1110address;
    public:
        ADS1110(byte address);
        ~ADS1110(void);
        byte   ping(void);
        byte   setGain(gain_t n_gain);
        byte   setRate(rate_t n_rate);
        byte   setMode(mode_t n_mode);
        byte   reset(void);
        String configInfo(void);
        int    readData(void);
        float  readVoltage(void);
        int    readPercentage(void);
        int    singleCon(void);
    private:
        void   initCall(void);
        byte   endCall(void);
        byte   config(void);
        byte   updateConfig(byte n_config);
        byte   dataRequest(void);
        byte   transErr(void);
        long   minCode(byte d_config);
};

extern ADS1110 Ads1110;

#endif