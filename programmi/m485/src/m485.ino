#include <LnFunctions.h>                //  D2X(dest, val, 2), printHex
#include <LnRS485_protocol.h>
#include <SoftwareSerial.h>

#include "RS-485_Slave.h"                      //  pin definitions
#include <EEPROM.h>


SoftwareSerial rs485    (6,5); //(2, 3);  // receive pin, transmit pin
const byte ENABLE_PIN   = 3; //4;

// callback routines
SoftwareSerial serialRs485 (RS485_RX_PIN, RS485_TX_PIN);  // receive pin, transmit pin

// callback routines
void fWrite (const byte what)   { rs485.write (what); }
int fAvailable ()               { return rs485.available (); }
int fRead ()                    { return rs485.read (); }

byte old_level = 0;

/* --------------------
    mi serve per verificare i dati e l'ordine con cui sono stati inviati inclusi STX, CRC, ETX
    DEBUG_TxRxMsg[0] contiene lunghezza dei dati
-------------------- */
byte DEBUG_TxRxMsg [200] = "                                                                ";   // gli faccio scrivere il messaggio inviato con relativo CRC
byte rxData         [60];

//--------------------------------------------------------------------------
void setup() {
    Serial.begin (9600);
    rs485.begin  (9600);
    pinMode (ENABLE_PIN, OUTPUT);   // driver output enable
    pinMode (LED_PIN, OUTPUT);      // built-in LED
}  // end of setup
//--------------------------------------------------------------------------

// #########################################################
// #
// #########################################################
void loop() {
    byte myAddress;        // who we are
    myAddress = EEPROM.read (0);

    int timeOut     = 10;       // tempo breve, ma a cosa serve?
    //byte buf [20];

    
    byte msg[200] = { "                            "} ; 

    digitalWrite (ENABLE_PIN, LOW);             // enable receiving
    recvMsg      (fRead, msg, sizeof(msg), DEBUG_TxRxMsg);
    digitalWrite (ENABLE_PIN, HIGH);            // disable receiving

    //Serial.print("-");

    if (msg[0] == 10) {             // ricevuto messaggio do addr 10

        Serial.println(F(" Ciao sono Arduino indirizzo 10!"));
        Serial.print("ricevuto:  ");
        Serial.print(msg[0]);Serial.print("-");Serial.print(msg[1]);Serial.print("-");
        Serial.print(msg[2]);Serial.print("-");Serial.print(msg[3]);Serial.print("-");
        Serial.print(msg[4]);Serial.print("-");Serial.print(msg[5]);Serial.println(".");
    } 


    // -------------- LORETO -------

   // delay (5000);  // give the master a moment to prepare to receive
}  // end of loop

