/*-----( Import needed libraries )-----*/
#include <SoftwareSerial.h>
#include <EEPROM.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define SSerialRX        6   //10  //Serial Receive pin     RO
#define SSerialTX        5   //11  //Serial Transmit pin    DI
#define SSerialTxControl 3   //RS485 Direction control

#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define Pin13LED         13

/*-----( Declare objects )-----*/
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

/*-----( Declare Variables )-----*/
int DatiDaSpedire[11];    // 11 byte (compreso lo 0)(usati da [1] a [10]) Vedi sotto per spegazione
int DatiRicevuti[11];     // 11 byte (compreso lo 0)(usati da [1] a [10]) Vedi sotto per spegazione

int dato = 0 ;          // uso generico
int uso = 0 ;           // uso generico
int Pnt1 = 0;           // serve per la funzione receive
byte myAddress ;        // indirizzo nella EEPROM
int stato = 0;          // come dato da usare

void setup() {  /****** SETUP: RUNS ONCE ******/
    Serial.begin(9600); // Start the serial port   
    Serial.println("Slave abilitato");  // Can be ignored  
    pinMode(Pin13LED, OUTPUT);   
    pinMode(SSerialTxControl, OUTPUT);  
    digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver 
    RS485Serial.begin(4800);   // Start the software serial port and set the data rate   era 300
   
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
// i dati sono per lo slave giusto e lo stato e' HIGH (dati letti)
    if (stato == HIGH) { 
    //    AVideoIDatiLetti();                 // a video i dati letti sulla linea
        stato = LOW ; // reset

        uso = uso + 1 ;
        if (uso >= 200) { uso= 2; }
        DatiDaSpedire[4] = uso;       
        digitalWrite(Pin13LED, HIGH); delay(10);  // Show activity


        if (DatiRicevuti[3] == EEPROM.read (0)) { // comandi per lo slave corretto?
            Serial.println("");
            Serial.print("EVVIVA! Il comando e' per me! "); Serial.println("[S1]"); 
            DatiDaSpedire[9] = 0x51 ; // segna la risposta

            SendDataTo485();
            AVideoDatiSpediti();   // stampa a video i dati spediti sulla linea
            digitalWrite(Pin13LED, LOW);  


        }   // fine IF iniziale
/*        else if (DatiRicevuti[3] != EEPROM.read (0)) { // slave sbagliato
            Serial.println("");
            Serial.println("chissa' per chi era il comando, NON per me di sicuro!!!!!!! "); Serial.println("[S1]"); 
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
