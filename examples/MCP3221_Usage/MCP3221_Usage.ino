#include "MCP3221.h"

const byte DEV_ADDR = 0x4D;                 // B01001101, DEC: 77 - I2C READ ADDRESS of the MCP3221 (Change as needed based on specific device part number)

//const byte DEV_CONVERSE = 0x4C;            // B01001100, DEC: 76 - I2C WRITE ADDRESS of the MCP3221 (Change as needed based on specific device part number)

                                            // (Another address option:  B10011011, 0x9B, DEC: 155 - Supposedly the READ address)

MCP3221 mcp3221(DEV_ADDR);                  // Constructs the MCP3221 object with the above address

unsigned int reading, reading12V;

void setup() {
    Serial.begin(9600);                 // Initiallizes the Serial Port (at 9600bd)
    Wire.begin();                       // Initiallizes the I2C communications bus
    while(!Serial);                     // Waits for Serial Port to initialize
    Serial.print("\nSerial Open\n");
}

void loop() {

  byte ping = mcp3221.ping();
  ping ? Serial.print("\No Response\n") : Serial.print("\nPong\n");
  delay(500);

//  reading = mcp3221.singleReading();
//  Serial.println("\nSingle Reading (5V): " + String(reading) + "mV\n");
//  delay(500);
//
//  reading12V = mcp3221.calc12V(reading);
//  Serial.println("\nReading 12V: " + String(reading12V) + "mV\n");
//  delay(500);
}

