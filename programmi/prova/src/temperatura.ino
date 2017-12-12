/* spiegazioni in fondo al codice */

#include <Wire.h>
#define THR_VALUE_HIGH      0x1E00 // gradi 30*256 se sale sopra questo valore alert=1       
#define THR_VALUE_LOW       0x1D00 // gradi 29*256 se scende sotto questo valore alert=0       
int SENSOR_ADDR = 0x48 ;
float temperatureHigh;
float temperatureLow ;

// ==========================================================
void setup() {
    Wire.begin();           // I2C start-up
    Serial.begin(9600);     // Serial start-up at 9600bps
    pinMode(7, INPUT);      // Set the Digital 7 pin as input
    digitalWrite(7, HIGH);  // Activate the pull-up resistor on Digital 7 pin
    //temperatureHigh = (THR_VALUE_HIGH >> 4)*0.0625f;
    //temperatureLow  = (THR_VALUE_LOW  >> 4)*0.0625f;
    sensorConfig();         // Configure the temperature sensor
}
// ========================================================== 
void loop() {

    sensorRead();    /* Read the temperature each delay time */  
    delay(2500); 
}
// ==========================================================
void sensorConfig() {
    i2cWrite8(0x01, 0x64);    /* 12-bit resolution, ALARM pin is active-high */
    i2cWrite16(0x02, THR_VALUE_LOW);	// Set values degrees for T-low threshold
    i2cWrite16(0x03, THR_VALUE_HIGH);	//  and  T-high threshold
}
// ==========================================================
void sensorRead() {
    unsigned short val;
    float temperature;
    int gradi = 0;
    int dec_C = 0;  //SS per display
    int uni_C = 0;  //SS per display 
    int vir_C = 0;  //SS per display     

    temperature = (i2cRead16(0x00) >> 4);
    temperature *= 0.0625f;
    
    Serial.print("Temp=");    Serial.print(temperature);
    Serial.print(" Alarm= "); Serial.println(digitalRead(7));
// ****** 
    gradi = (temperature * 10);               // esempio : gradi diventa da 27,5   a   275
    dec_C = (gradi - (gradi % 100)) / 100;  // (275 - (75)) /100  = 2
    gradi = gradi - (dec_C * 100);          // 275 - (2 * 100) = 75
    uni_C = (gradi - (gradi % 10)) / 10;    // (75 - (5)) /10     = 7
    gradi = gradi - (uni_C * 10);           // 75 -(7 * 10)
    vir_C = gradi;                          // 5 = 5

    Serial.print(temperature); Serial.print("->"); Serial.print(dec_C); 
    Serial.print(uni_C); Serial.print(","); Serial.print(vir_C); Serial.println(" °C");
    Serial.println("-----------------------------------------");Serial.println(" ");Serial.println(" ");
}
// ==========================================================
static unsigned short i2cRead16(byte reg) {     // Reads two bytes for the given register 
    unsigned short val;
    Wire.beginTransmission(SENSOR_ADDR);
    Wire.write(reg);
    Wire.endTransmission(true);
    Wire.requestFrom(SENSOR_ADDR, 2);
  
    val = Wire.read() << 8;
    val |= Wire.read();
    return val;
}
// ==========================================================
static void i2cWrite16(byte reg, unsigned short value) {    // Writes two bytes to the given register
    Wire.beginTransmission(SENSOR_ADDR);
    Wire.write(reg);
    Wire.write(value >> 8);
    Wire.write(value & 0xFF);
    Wire.endTransmission(true);
}
// ==========================================================
static void i2cWrite8(byte reg, byte value) {   // Writes a byte to the given register
    Wire.beginTransmission(SENSOR_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission(true);
}

// ==========================================================
/*
 * ==========================================================
 * Artekit demo sketch for the AK-AT30TS75 Temperature sensor
 * breakout board.
 *
 * Copyright (c) 2013 Artekit Italy
 * http://www.artekit.eu
 *
 * Connect the pins in the following way:
 *
 * Arduino UNO          AK-AT30TS75
 * ==================================
 *   AN5                   SCL
 *   AN4                   SDA
 *   Digital 7             ALERT
 *   5V                    VCC
 *   GND                   GND
 *
 * This sketch assumes the AK-AT30TS75 A0,A1 and A2 pins are
 * connected to GND.
 *
 * This demonstration outputs the current 12-bit temperature
 * value and the ALERT pin state. 
 *
 * The ALERT pin will go HIGH when a temperature of 30 Celsius
 * degrees is reached. In this state, the ALERT pin will go back
 * DOWN when the temperature is lower than 29 Celsius degrees.
 *
 * ==========================================================
 */

//-------------------------------------------------------------
    /* 12-bit temperature is read/written using the formula:
     * 
     * (temp / 0.0625) << 4
     *
     * temp = temperature
     * 0.0625 = resolution for 12-bit values
     */