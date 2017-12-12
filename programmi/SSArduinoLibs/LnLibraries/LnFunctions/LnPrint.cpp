
#include <LnFunctions.h>
/*
void LnPrint(const char *data1, const char *data2, const char *data3) {
    Serial.print(data1);
    Serial.print(data2);
    Serial.print(data3);
}

*/

void printNchar(const char data, byte counter) {
    byte i;
    for (i=0; i<counter; i++) {
        Serial.print(data);
    }
}


// ---------------------------------------
// stampa solo char visualizzabili
// se la len==0 allora calcoliamo la
// lunghezza cercando il '\0' nella stringa.
// ---------------------------------------
void printDelimitedStr(const char *data, byte len, const char *delimiter) {
    byte i;
    if (delimiter) Serial.print(delimiter[0]);

    // - calcolo len
    if (len==0) len = stringLen(data);

    // - print data
    for (i=0; i<len; i++) {
        if ( (data[i]>=32) & (data[i]<127))
            Serial.print(char(data[i]));
        else if ( (data[i]==10) & (data[i]==13))
            Serial.print(char(data[i]));
        else
            Serial.print(' ');
    }

    if (delimiter) Serial.print(delimiter[1]);
}


