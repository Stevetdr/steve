#include <Arduino.h>
#include <LnFunctions.h>                //  D2X(dest, val, 2), printHex
#include <LnRS485_protocol.h>
#include <SoftwareSerial.h>
#include "RS-485_Slave.h"                      //  pin definitions
#include <EEPROM.h>
void fWrite(const byte what);
int  fAvailable();
int  fRead();
void setup();
void loop();
void processRequest(RXTX_DATA *pData);
void sendMessage(byte destAddr, byte data[], byte dataLen, RXTX_DATA *pData);
void rxDisplayData(byte rCode, RXTX_DATA *pData);
void txDisplayData(byte rCode, RXTX_DATA *pData);
#line 1 "src/RS-485_Slave.ino"
/*
    http://www.gammon.com.au/forum/?id=11428
*/

//#include <LnFunctions.h>                //  D2X(dest, val, 2), printHex
//#include <LnRS485_protocol.h>
//#include <SoftwareSerial.h>

//#include "RS-485_Slave.h"                      //  pin definitions

//#include <EEPROM.h>

// Author:             Loreto notarantonio
char myVersion[] = "LnVer_2017-04-13_17.07.24";

SoftwareSerial serialRs485 (RS485_RX_PIN, RS485_TX_PIN);  // receive pin, transmit pin

// ------ callback routines
void fWrite(const byte what)   {       serialRs485.write (what); }
int  fAvailable()              {return serialRs485.available (); }
int  fRead()                   {return serialRs485.read (); }

// void fWrite(const byte what)  {Serial.write (what); }
// int fAvailable()              {return Serial.available (); }
// int fRead()                   {return Serial.read (); }

#define DATALEN     0
#define SENDER      1
#define DESTINATION 2
#define PAYLOAD     3

byte        myEEpromAddress;        // who we are
RXTX_DATA   RxTx;             // struttura dati
//.............0         1
//.............01234567890123
char myID[] = "Arduino: xxx :";



//python3.4 -m serial.tools.list_ports
void setup() {
    RxTx.displayData = true;    // data display dei byt hex inviatie ricevuti (lo fa direttamente la libreria)

    Serial.begin(9600);             // SERIAL_8N1 (the default)
    serialRs485.begin(9600);
    pinMode (RS485_ENABLE_PIN, OUTPUT);  // driver output enable
    pinMode (LED_PIN, OUTPUT);          // built-in LED

    /* ------------------------
        preparazione myID con indirizzo
        1. convert integer myAddress to string
        2. copy string into myID array
    */
    myEEpromAddress = EEPROM.read(0);
    char *xx = LnUtoa(myEEpromAddress, 3, '0');
    myID[9]  = xx[0];
    myID[10] = xx[1];
    myID[11] = xx[2];

    // delay(5*1000);

    Serial.println(myID);


}


// ################################################################
// #
// ################################################################
void loop() {
    RXTX_DATA *pData;
    pData = &RxTx;
    pData->timeout = 10000;
    byte rCode = recvMsg (fAvailable, fRead, pData);

    // displayData(rCode, pData)
    if (rCode == LN_OK) {
        processRequest(pData);

            // print solo se non lo ha già fatto al libreria
    }
    else if (pData->rx[0] == 0) {
        Serial.print(myID);
        Serial.print(" rCode: ");Serial.println(rCode);
        Serial.print(F("Nessuna richiesta ricevuta in un tempo di: "));
        Serial.print(pData->timeout);
        Serial.println(F("mS"));
    }

}

// #############################################################
// #
// #############################################################
void processRequest(RXTX_DATA *pData) {
    // byte dataLen    = pData->rx[DATALEN];
    byte senderAddr = pData->rx[SENDER];
    byte destAddr   = pData->rx[DESTINATION];

    if (destAddr == myEEpromAddress) {    // sono io.... rispondi sulla RS485
        Serial.println(F("Request is for me...: answering..."));
        byte response[] = "Loreto";
        delay(1000);
        sendMessage(senderAddr, response, sizeof(response), pData);
    }
    else {                                // non sono io.... commento sulla seriale
        Serial.println();
        Serial.print(" rCode: ");Serial.println(LN_OK);
        printHexPDS (" source address: ", senderAddr);
        printHexPDS (" destAddr      : ", destAddr);
        Serial.println(F("Request is NOT for me...: "));
        // printHex(&pData->rx[1], dataLen, "\r\n");
        rxDisplayData(0, pData);
    }
}


// #############################################################
// #
// #############################################################
void sendMessage(byte destAddr, byte data[], byte dataLen, RXTX_DATA *pData) {
    pData->tx[SENDER]       = myEEpromAddress;    // SA
    pData->tx[DESTINATION]  = destAddr;    // DA

    byte index = PAYLOAD;
    for (byte i = 0; i<dataLen; i++)
        pData->tx[index++] = data[i];         // copiamo i dati nel buffer da inviare

    pData->tx[DATALEN] = index;  // set dataLen

        // send to RS-485 bus
    digitalWrite(RS485_ENABLE_PIN, HIGH);               // enable sending
    sendMsg(fWrite, pData);
    digitalWrite(RS485_ENABLE_PIN, LOW);                // disable sending
}

// ################################################################
// # --- DISPLAY DATA
// ################################################################

void rxDisplayData(byte rCode, RXTX_DATA *pData) {
    displayDebugMessage("inoRECV-data", rCode, pData->rx);
    displayDebugMessage("inoSEND-raw ", rCode, pData->raw);
}
void txDisplayData(byte rCode, RXTX_DATA *pData) {
    displayDebugMessage("inoSEND-data", rCode, pData->tx);
    displayDebugMessage("inoSEND-raw ", rCode, pData->raw);
}
