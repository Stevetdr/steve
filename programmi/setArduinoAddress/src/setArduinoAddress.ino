
// impostazione di un indirizzo fisso in EEPROM. Comodo per Rs-485
#include <EEPROM.h>

const int myAddress        = 0x0B;       // valore da mettere in byte 0 della EEPROM
//const byte maxAddress       = 20;       // valore da mettere in byte 1 della EEPROM
//---------------------------------------------------------------------------------
const bool updateAddress    = false;    // per leggere l'indirizzo di Arduino
//const bool updateAddress    = true;     // per settare l'indirizzo in Arduino
//---------------------------------------------------------------------------------
void setup () {

/*    if (updateAddress) {                    // se undateAddress = true esegue sotto
        if (EEPROM.read  (0) != myAddress)
            EEPROM.write (0, myAddress);
  //      if (EEPROM.read  (1) != maxAddress)
  //          EEPROM.write (1, maxAddress);
    }
*/
    EEPROM.write (0, myAddress);

    Serial.begin (9600);                // debugging prints

}  // end of setup



void loop () {
// from EEPROM
int myAddress;         // who we are
byte maxAddress;        // maximum devices on the bus

    // debugging prints
    myAddress = EEPROM.read (0);
    Serial.print (F("My address    is : "));
    Serial.println (myAddress,HEX);
/*
    maxAddress = EEPROM.read (1);
    Serial.print (F("Max addresses is : "));
    Serial.println (int (maxAddress));
*/
    delay(2000);
}

/*
    http://www.gammon.com.au/forum/?id=11428

*/