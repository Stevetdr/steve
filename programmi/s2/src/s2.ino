/*-----( Import needed libraries )-----*/
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Wire.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define SSerialRX        6   //10  //Serial Receive pin     RO
#define SSerialTX        5   //11  //Serial Transmit pin    DI
#define SSerialTxControl 3   //RS485 Direction control

#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define Pin13LED         13
// sensore temperatura AK-AT30TS75 vedi programma temperatura.ino per ulteriori spiegazioni
#define THR_VALUE_HIGH      0x1E00 // gradi 30*256 se sale sopra questo valore alert=1       
#define THR_VALUE_LOW       0x1D00 // gradi 29*256 se scende sotto questo valore alert=0       
int SENSOR_ADDR = 0x48 ; // indirizzo del sensore di temperatura con A0 1 e 2 a GND 

SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, SX     /*-----( Declare objects )-----*/

/*-----( Declare Variables )-----*/
int DatiDaSpedire[11];    // 11 byte (compreso lo 0)(usati da [1] a [10]) Vedi sotto per spegazione
int DatiRicevuti[11];     // 11 byte (compreso lo 0)(usati da [1] a [10]) Vedi sotto per spegazione

int dato = 0 ;          // uso generico
int uso = 0 ;           // uso generico
int Pnt1 = 0;           // serve per la funzione receive
//byte myAddress ;        // indirizzo nella EEPROM
int stato = 0;          // come dato da usare per valutare se o meno ci sono dati

float temperature;      // gestione calcoli temperatura
int gradi = 0;          // gestione calcoli temperatura
int dec_C = 0;          // gestione calcoli temperatura
int uni_C = 0;          // gestione calcoli temperatura
int vir_C = 0;          // gestione calcoli temperatura

void setup() {  /****** SETUP: RUNS ONCE ******/
    Wire.begin();                       // I2C start-up
    Serial.begin(9600);                 // Start the serial port   
    Serial.println("Slave abilitato");  // Can be ignored  
    pinMode(Pin13LED, OUTPUT);          // led
    pinMode(7, INPUT);                  // Set the Digital 7 pin as input per leggere ALERT sensore temperatura, sacrificabile
    digitalWrite(7, HIGH);              // Activate the pull-up resistor on Digital 7 pin                              ""
    pinMode(SSerialTxControl, OUTPUT);  
    digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver 
    RS485Serial.begin(4800);   // Start the software serial port and set the data rate   era 300
    sensorConfig();         // Configure the temperature sensor AK-AT30TS75
   
    DatiDaSpedire[1] = 0x02;   // start of data
    DatiDaSpedire[2] = EEPROM.read (0);   // si carica l'indirizzo di se' stesso
    DatiDaSpedire[3] = 0x10;   // indirizzo del master, sempre 10
    DatiDaSpedire[4] = 0x00; DatiDaSpedire[5] = 0x00; DatiDaSpedire[6] = 0x00;   // dato generico
    DatiDaSpedire[7] = 0x00; DatiDaSpedire[8] = 0x00; DatiDaSpedire[9] = 0x00;   // dato generico 
    DatiDaSpedire[10] = 0x03;   // end of data

    Serial.print (F("Slave: il mio indirizzo e' ")); Serial.println(DatiDaSpedire[2],HEX); Serial.println("");  

    while (RS485Serial.available()) { dato = RS485Serial.read(); } // reset del contenuto del buffer    

}   //--(end setup )---
//***********************************************************************************************************




void loop() {  /****** LOOP: RUNS CONSTANTLY ******/
//***********************************************************************************
	// lettura dei dati trasmessi da tutti
    while (RS485Serial.available()) {       // c'e' qualche dato nel buffer?
        ReceiveDataFrom485();               // alla lettura
    //    AVideoIDatiLetti();                 // a video i dati letti sulla linea
        stato = HIGH ;                      // dati letti
    }

        //Serial.print("9  "); Serial.println(DatiDaSpedire[9]);
        //    while (RS485Serial.available()  ) {
    //    DatiDaSpedire[9] = 0x00 ; // reset la risposta

    //    if (DatiRicevuti[3] == EEPROM.read (0) && stato == HIGH )     // indirizzo dello slave
    //        { Serial.println("A"); DatiDaSpedire[9] = 0x11 ;}  // rilevato indirizzo dello slave corretto 
        //        }

//*********************************************************************************** 
// i dati sono per lo slave giusto e lo stato e' HIGH (dati letti) ?
    if (stato == HIGH) { 
        //AVideoIDatiLetti();                 // a video i dati letti sulla linea
        stato = LOW ; // reset

        uso = uso + 1 ;
        if (uso >= 200) { uso= 2; }
        DatiDaSpedire[4] = uso;       
        digitalWrite(Pin13LED, HIGH); delay(10);  // Show activity


        if (DatiRicevuti[3] == EEPROM.read (0)) { // comandi per lo slave corretto?
            Serial.println("");
            Serial.print("EVVIVA! Il comando e' per me! "); Serial.println("[S2]"); 
            DatiDaSpedire[9] = 0x51 ; // segna la risposta
            
            sensorRead();    // legge la temperatura e la visualizza a video, scompone i singoli byte per la trasmissione
                DatiDaSpedire[6] = 0x01;    // segno =1 se positivo  =0 se negativo
                DatiDaSpedire[7] = dec_C;   // decine di unita' della temperatura
                DatiDaSpedire[8] = uni_C;   // unita' della temperatura
                DatiDaSpedire[9] = vir_C;   // cifra dopo la virgola della temperatura

            SendDataTo485();
            AVideoDatiSpediti();   // stampa a video i dati spediti sulla linea
            digitalWrite(Pin13LED, LOW);  


        }   // fine IF iniziale
 /*       else if (DatiRicevuti[3] != EEPROM.read (0)) { // slave sbagliato
            Serial.println("");
            Serial.println("chissa' per chi era il comando, NON per me di sicuro!!!!!!! "); Serial.println("[S2]"); 
            DatiDaSpedire[9] = 0x00 ; // reset la risposta }
        }   // fine ELSE IF iniziale
*/

//        SendDataTo485();
//        AVideoDatiSpediti();   // stampa a video i dati spediti sulla linea
//        digitalWrite(Pin13LED, LOW);           

    } 

}       //--(end main loop )---


//*********************************************************************************************************
//*********************************************************************************************************
//*********************************************************************************************************
void SendDataTo485() {   // Send bytes on line 485 ========================================

    digitalWrite(SSerialTxControl, RS485Transmit);   // Enable RS485 Transmit
    delay(10);
    for (int x=1; x < 11 ;x++) { RS485Serial.write(DatiDaSpedire[x]); }                 
    delay(10);
    digitalWrite(SSerialTxControl, RS485Receive);    // Disable RS485 Transmit
}
//********************************************************************************************************* 
void ReceiveDataFrom485() {

    Pnt1 = 1 ;  // reset del pointer della matrice di scrittura
    while (RS485Serial.available()) {                     
        delay(10);  // serve per permettere il completamento dell'operazione di lettura o scrittura
        DatiRicevuti[Pnt1] = RS485Serial.read() ;     // legge e scrive nella matrice dal byte 1
        //Serial.println(DatiRicevuti[Pnt1],HEX);
        Pnt1 = Pnt1 + 1;
    }
    delay(10); 
}
//*********************************************************************************************************
void AVideoIDatiLetti() {           // a video i dati letti sulla linea
    Serial.println("Ricevuto dal master i seguenti dati : --------------------------------------------");

    for (int x=1; x < 11 ;x++) { Serial.print("[");Serial.print(x);Serial.print("]");Serial.print("\t");} 
    Serial.println(""); 
    for (int x=1; x < 11 ;x++) { Serial.print(DatiRicevuti[x],HEX);Serial.print("\t");}   
    Serial.println(""); 
    Serial.println("--------------------------------------------------------------fine dati dal master");Serial.println(" ");
}
//*********************************************************************************************************
void AVideoDatiSpediti() {   // stampa a video i dati spediti sulla linea        
    Serial.println("inviato al master ->   ------------------------------------------------inizio dati");

    for (int x=1; x < 11 ;x++) { Serial.print("[");Serial.print(x);Serial.print("]");Serial.print("\t");} 
    Serial.println(""); 
    for (int x=1; x < 11 ;x++) { Serial.print(DatiDaSpedire[x],HEX);Serial.print("\t");}   
    Serial.println(""); 
    Serial.println("-------------------------------------------------------------------------fine dati");Serial.println(" ");  
    Serial.println(" ");  Serial.println(" ");  Serial.println(" ");          
/*
        Serial.print("Variabili:");
        Serial.print ("    stato= "); Serial.print (stato); 
        Serial.print ("   uso= "); Serial.print (uso);  
        Serial.print ("   available= "); Serial.println (RS485Serial.available()); 
*/
}
//********************************************************************************************************
// ==========================================================
void sensorConfig() {
    i2cWrite8(0x01, 0x64);    /* 12-bit resolution, ALARM pin is active-high */
    i2cWrite16(0x02, THR_VALUE_LOW);    // Set values degrees for T-low threshold
    i2cWrite16(0x03, THR_VALUE_HIGH);   //  and  T-high threshold
}
// ==========================================================
void sensorRead() {
    //unsigned short val;

    //float temperature;
    //int gradi = 0;
    //int dec_C = 0;  //SS per display
    //int uni_C = 0;  //SS per display 
    //int vir_C = 0;  //SS per display     

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