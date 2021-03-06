#include <Arduino.h>
#include <Wire.h>
#include "LedControl.h"        //SS
void sensorRead();
void OnDisplay(float temperature);
void setup();
void loop();
static unsigned short i2cRead16(byte reg);
static void i2cWrite16(byte reg, unsigned short value);
static void i2cWrite8(byte reg, byte value);
void sensorConfig();
#line 1 "src/temperatura.ino"
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
 * This sketch assumes the AK-AT30TS75 A0,A1 and A2 pins are connected to GND.   HEX 48
 * This demonstration outputs the current 12-bit temperature value and the ALERT pin state.
 * The ALERT pin will go HIGH when a temperature of 30 Celsius degrees is reached. 
 * In this state, the ALERT pin will go back DOWN when the temperature is lower than 29 Celsius degrees.
 *
 * ==========================================================
 */

//#include <Wire.h>
//#include "LedControl.h"        //SS

LedControl lc=LedControl(12,11,10,1);   //SS

bool fDEBUG = true;
//#define SENSOR_ADDR_in 0x48 //sono i 7 bit dell'indirizzo piu', all'inizio uno zero 0 1001 000 ovvero 0100 1000 hex 48
#define ALERT_PIN  7
#define led13     13
#define ledred     8
#define THR_VALUE_LOW       19*256
#define THR_VALUE_HIGH      20*256

int SENSOR_ADDR = 0x4B; // non capisco ma funziona con A2=A1=A0=GND hex 48    
float temperatureHigh;
float temperatureLow ;

//================================================================================
void sensorRead() {
    unsigned short val;
    //float temperature_in;
    //float temperature_out;
    float temperature;
    //int myALARM;
    //SENSOR_ADDR = SENSOR_ADDR_in; 
    temperature = (i2cRead16(0x00) >> 4);
    temperature *= 0.0625f;
    OnDisplay(temperature);    
    /*if (temperature > temperatureHigh)
        myALARM = 1;
    else
        myALARM = 0;

    if (fDEBUG) {
        Serial.print(" SS Temperature (Min:");Serial.print(temperatureLow);
        Serial.print(" Max:"); Serial.print(temperatureHigh);
        Serial.print(") - "); Serial.print(temperature);
        Serial.print(" - Alarm:");Serial.print(!digitalRead(ALERT_PIN));
        Serial.print(" - myALARM:");Serial.println(myALARM);
    }*/
   
}
//================================================================================

void OnDisplay(float temperature){  
    int gradi;
    int dec_C;  //SS per display
    int uni_C;  //SS per display 
    int vir_C;  //SS per display     
// ****** 
  gradi = (temperature * 10);               // esempio : gradi diventa da 27,5   a   275
  dec_C = (gradi - (gradi % 100)) / 100;  // (275 - (75)) /100  = 2
  gradi = gradi - (dec_C * 100);          // 275 - (2 * 100) = 75
  uni_C = (gradi - (gradi % 10)) / 10;    // (75 - (5)) /10     = 7
  gradi = gradi - (uni_C * 10);           // 75 -(7 * 10)
  vir_C = gradi;                          // 5 = 5

    lc.setChar(0,7,' ',false);	// se si usa un display esterno
    lc.setChar(0,6,' ',false);           //setChar(addr,digit,value,dp)
    lc.setChar(0,5,' ',false);
    lc.setChar(0,3,'C',false);
    lc.setDigit(0,2,(byte)dec_C,false);
    lc.setDigit(0,1,(byte)uni_C,true);
    lc.setDigit(0,0,(byte)vir_C,false);
    //lc.setDigit(0,4,'4',false);
    delay(2000);
    Serial.print(temperature); Serial.print("->"); Serial.print(dec_C); 
    Serial.print(uni_C); Serial.print(","); Serial.print(vir_C); Serial.println(" °C");

 lc.clearDisplay(0);
// ****** 
}
//================================================================================
void setup() {
 /*  The MAX72XX is in power-saving mode on startup,     //SS   */
  lc.shutdown(0,false);             /* We have to do a wakeup call  */
  lc.setIntensity(0,4);             /* Set the brightness to a medium values era 8*/
  lc.clearDisplay(0);               /* and clear the display */

    Wire.begin();    /* I2C start-up */

    Serial.begin(9600);     // Debug    /* Serial start-up at 9600bps */

Serial.println("setup");
    /* Set the Digital ALERT_PIN as input */
    // pinMode(ALERT_PIN, INPUT);
    pinMode(ALERT_PIN, INPUT_PULLUP);
    pinMode(ledred,OUTPUT);

    /* Activate the pull-up resistor on Digital ALERT_PIN */
    // digitalWrite(ALERT_PIN, LOW);

    temperatureHigh = (THR_VALUE_HIGH >> 4)*0.0625f;
    temperatureLow  = (THR_VALUE_LOW  >> 4)*0.0625f;

    sensorConfig();    /* Configure the temperature sensor */
}

void loop() {

    sensorRead();       /* Read the temperature each second */
    
    digitalWrite(led13, HIGH); delay(300); digitalWrite (led13, LOW); delay(300);
    digitalWrite(ledred, HIGH); delay(300); digitalWrite (ledred, LOW);
}
//===============================================================================
//--------------------------------------------------------------------------------------
/* This function reads two bytes for the given register */
static unsigned short i2cRead16(byte reg) {
Serial.print("i2cRead16");
    unsigned short val;
    Wire.beginTransmission(SENSOR_ADDR); // manda i primi sette bit
    Wire.write(reg);
    Wire.endTransmission(true);
    Wire.requestFrom(SENSOR_ADDR, 2);

    val = Wire.read() << 8;
    val |= Wire.read();
Serial.print("val= ");Serial.println(val);
    return val;
}
//--------------------------------------------------------------------------------------
/* This function writes two bytes to the given register */
static void i2cWrite16(byte reg, unsigned short value) {
Serial.println("i2cWrite16");
    Wire.beginTransmission(SENSOR_ADDR);
    Wire.write(reg);
    Wire.write(value >> 8);
    Wire.write(value & 0xFF);
    Wire.endTransmission(true);
}
//--------------------------------------------------------------------------------------
/* This function writes a byte to the given register */
static void i2cWrite8(byte reg, byte value) {
Serial.print("i2cWrite8"); Serial.print(" reg, value, sensor_addr= ");Serial.print(reg);Serial.print("-");Serial.print(value);Serial.print("-");Serial.println(SENSOR_ADDR);
    Wire.beginTransmission(SENSOR_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission(true);
}
//--------------------------------------------------------------------------------------
void sensorConfig() {
Serial.println("routine sensorConfig");
    // ==============================================================
    //SENSOR_ADDR = SENSOR_ADDR_in;  // valore in (0x48) in variabile
    /* 12-bit resolution, ALARM pin is active-high */
    i2cWrite8(0x01, 0x4B); //64

    /* 12-bit temperature is read/written using the formula:
    *
    * (temp / 0.0625) << 4
    *
    * temp = temperature
    * 0.0625 = resolution for 12-bit values
    */

    // Set values degrees for T-low threshold  and  T-high threshold
    i2cWrite16(0x02, THR_VALUE_LOW);
    i2cWrite16(0x03, THR_VALUE_HIGH);
    // ==============================================================
    //SENSOR_ADDR = SENSOR_ADDR_out;  // valore out (0x48) in variabile
    i2cWrite8(0x01, 0x4B);	//64
    i2cWrite16(0x02, THR_VALUE_LOW);
    i2cWrite16(0x03, THR_VALUE_HIGH);
}