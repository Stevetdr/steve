// definizione pin I/O su Arduino nano  --
int SpiaR   =  3 ;            // spia led ROSSO esterno
int SpiaV   =  4 ;            // spia led VERDE esterno
int SpiaB   =  5 ;            // spia BUZZER esterno
int SpiaA   =  6 ;            // spia ENTRATA AMIGA da scheda OC
int Poc     =  7 ;            // definizione input digitale su piedino 13 - collegato a oc antifurto
int DisaLed =  8 ;            // definizione pin per abilitare o meno il blink dei led spia
int PWM1    =  9 ;            // definisce output PWM su piedino PWM1 led verde
int PWM2    = 10 ;            // definisce output PWM su piedino PWM2 led rosso
int Buzz    = 11 ;            // definizione output digitale su piedino 14 - collegato al buzzer
int Led13   = 13 ;            // definizione per il led sulla scheda
int FotoR   = A5 ;            // definizione input analogico su piedino 15 - collegato alla fotoresistenza
// definizioni e valorizzazione variabili
int suono = 0 ;              // identifica lo stato all'inizio, viene immediatamente modificato
int StatoOc = 0 ;            // valore dello stato del pin di oc dell'allarme
int StatoLed = 0 ;           // valore del jumper per abilitare il led 13. A + led acceso, a - led spento
int StatoPorta = 0 ;         // indica lo stato della porta per lampeggiare il led rosso o il verde
byte ValFotoR = 0 ;           // vaolore letto sul pin della fotoresistenza da 0 a 1024 (e' un intero)
byte ValFotoR1 = 0;

int ON = 1 ;                 // generico per ON  = 1
int OFF = 0 ;                // generico per OFF = 0
int SpiaRs = 0;              // valore dello stato delle lampade Rosse
int SpiaVs = 0;              // valore dello stato delle lampade Verde

void setup() {
    pinMode(Led13, OUTPUT);     // imposta il pin digitale 13 come output per spia      pin 13
    pinMode(PWM1, OUTPUT);      // imposta il pin digitale come output                  pin digitale  OUT e' digitale o analogico per il PWM?
    pinMode(PWM2, OUTPUT);      // imposta il pin digitale come output                  pin digitale  OUT   "       "       "      "      "
    pinMode(SpiaR, OUTPUT);     // imposta il pin digitale come output                  pin digitale  OUT
    pinMode(SpiaV, OUTPUT);     // imposta il pin digitale come output                  pin digitale  OUT
    pinMode(SpiaB, OUTPUT);     // imposta il pin digitale come output                  pin digitale  OUT
    pinMode(SpiaA, OUTPUT);     // imposta il pin digitale come output                  pin digitale  OUT
    pinMode(Buzz, OUTPUT);      // piedino collegato al TR del come output              pin digitale  OUT
    pinMode(Poc, INPUT);        // piedino collegato a Open Collector dell'antifurto    pin digitale  IN
                                //  collegamento tramite res da 10K all OC dell'allarme
    pinMode(DisaLed, INPUT);    // se OFF non accende i led, se ON li accende tutti per test

    pinMode(FotoR, INPUT);      // piedino collegato alla fotoresistenza                pin analogico IN

    Serial.begin(9600);     // Debug   Serial start-up at 9600bps
}
//===============================================================================================


//========= partenza ============================================================================
void loop() {
    while (1) {      //loop infinito

        StatoOc = digitalRead(Poc);     // legge il pin del oc dell'antifurto (0 o 1)                                       //
        StatoLed = digitalRead(DisaLed); // legge il pin del junper per il blink dei led

        //Serial.print("StatoOc: ");Serial.print(StatoOc);Serial.print(" | ");
        //Serial.print("StatoLed: ");Serial.print(StatoLed);Serial.print(" | ");
        //Serial.print("suono: ");Serial.print(suono);Serial.print(" | ");
        //Serial.print("Val fotor: ");Serial.println(ValFotoR);

        //Serial.print(StatoLed);
//===== esame di un eventuale cambio dello stato del pin dell'allarme ============================
//= serve per fare suonare il beeper solo al cambio stato e non sempre ===========================
        //Serial.print(suono);
        if (StatoOc == ON) {                 // vuol dire allarme inserito       da verificare sul campo!!!!
            if (suono == 0 or suono == 2 ) {            // mai suonato fino ad ora ?
                Suona_Led_Rosso();    // costruire sequenza suoni
                suono = 1 ;           // suonato codice per inserimento e impostato variabile
                analogWrite(PWM1, 000); // reset
                analogWrite(PWM2, 000); // reset
                SpiaRs = 0 ;            // reset
                SpiaVs = 0 ;            // reset
            }
        }
        if (StatoOc == OFF) {                 // vuol dire allarme disinserito       da verificare sul campo!!!!
            if (suono == 0 or suono == 1 ) {            // mai suonato fino ad ora ?
                Suona_Led_Verde();    // costruire sequenza suoni
                suono = 2;            // suonato codice per inserimento e impostato variabile
                analogWrite(PWM1, 000); // reset
                analogWrite(PWM2, 000); // reset
                SpiaRs = 0 ;            // reset
                SpiaVs = 0 ;            // reset
            }
        }
//=================================================================================================
//=================================================================================================
    if (StatoLed == 1) {    // si usa il dipswitch per decidere se o meno abilitare i led spia
        LedBlink13();                    // faccio lampeggiare il led su scheda
        digitalWrite(SpiaR, SpiaRs);     // Spia led ROSSO
        digitalWrite(SpiaV, SpiaVs);     // Spia led VERDE
        digitalWrite(SpiaA, StatoOc);    // Spia dello stato dell OC di AMIGA
    }
    else if (StatoLed == 0) {    // disabilitati
        digitalWrite(SpiaR, OFF);     // Spia led ROSSO
        digitalWrite(SpiaV, OFF);     // Spia led VERDE
        digitalWrite(SpiaA, OFF);     // Spia dello stato dell OC di AMIGA
    }
//=================================================================================================
//=================================================================================================
//= in questa parte viene esaminato la variabile suono per il lampeggio ===========================
    if (suono == 1) {               // se suono e' =1 allarme inserito
        if (StatoPorta == OFF) {
            analogWrite(PWM1, 000 );    // // spegne LED ROSSO
            StatoPorta = ON;            // inverte luce verde : accesa o spenta
            SpiaRs = 1 ;                // led spia
        }
        else if(StatoPorta == ON) {
            analogWrite(PWM1, ValFotoR);  // accende LED ROSSO secondo la luminosita'
            StatoPorta = OFF;           // inverte luce verde : accesa o spenta
            SpiaRs = 0 ;                // led spia
        }
    delay(500);                 // attesa
    }
    if (suono == 2) {               // se suono e' =2 allarme disinserito
        if (StatoPorta == OFF) {
            analogWrite(PWM2, 000 );     // spegne  LED VERDE
            StatoPorta = ON ;           // inverte luce verde : accesa o spenta
            SpiaVs = 1 ;                // led spia
        }
        else if(StatoPorta == ON) {
            analogWrite(PWM2, ValFotoR);  // accende  LED VERDE secondo la luminosita'
            StatoPorta = OFF;           // inverte luce verde : accesa o spenta
            SpiaVs = 0 ;                // led spia
        }
    delay(500);                 // frequenza lampeggio led esterni
    }
//=================================================================================================
// il secondo campo dell'istruzione analogWrite indica la percentuale del PWM. Se e'=0 non c'e' PWM
//  (luce spenta) se e'=255 il PWM e' a 100 (accesa), se e'=128 e' al 50%. Vedi sotto!
    ValFotoR = byte(analogRead(FotoR) / 4 );    // lettura del valore della fotoresistenza sul pin da 0 a 1024
    Serial.print("valore   LDR: ");Serial.println(ValFotoR);
    if (ValFotoR>127) ValFotoR = 127;
    if (ValFotoR<10) ValFotoR = 10;
    ValFotoR1 = ValFotoR*2;
    //ValFotoR1 = map(ValFotoR, 0, 120, 20, 255);
    Serial.print("remapped LDR: ");Serial.println(ValFotoR1);
    Serial.println("");
                                               //  poi divide per 4 per portarlo nel range da 0 a 255
//=================================================================================================
    }       // fine While
//=================================================================================================
}           // fine Loop
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************


//===============================================================================================
void LedBlink13() {         // attesa di un 7/10 di secondo delay(700)
    digitalWrite(Led13, HIGH);  // accende il LED
    delay(100);                 // aspetta
    digitalWrite(Led13, LOW);   // spegne il LED
    delay(100);                 // aspetta
}
//===============================================================================================
void Suona_Led_Rosso(void) {        // inserimento allarme  4 beep da 300 ms con intervallo di 300 ms
    digitalWrite(Buzz, HIGH); if (StatoLed == 1) { digitalWrite(SpiaB,ON);} ;
    delay(300);
    digitalWrite(Buzz, LOW);  if (StatoLed == 1) { digitalWrite(SpiaB,OFF);} ;
    delay(300);
    digitalWrite(Buzz, HIGH); if (StatoLed == 1) { digitalWrite(SpiaB,ON);} ;
    delay(300);
    digitalWrite(Buzz, LOW);  if (StatoLed == 1) { digitalWrite(SpiaB,OFF);} ;
    delay(300);
    digitalWrite(Buzz, HIGH); if (StatoLed == 1) { digitalWrite(SpiaB,ON);} ;
    delay(300);
    digitalWrite(Buzz, LOW);  if (StatoLed == 1) { digitalWrite(SpiaB,OFF);} ;
    delay(300);
    digitalWrite(Buzz, HIGH); if (StatoLed == 1) { digitalWrite(SpiaB,ON);} ;
    delay(300);
    digitalWrite(Buzz, LOW);  if (StatoLed == 1) { digitalWrite(SpiaB,OFF);} ;
}
//===============================================================================================
void Suona_Led_Verde(void) {        // disinserimento allarme 3 beep due lunghi uno corto
    digitalWrite(Buzz, HIGH); if (StatoLed == 1) { digitalWrite(SpiaB,ON);} ;
    delay(1000);
    digitalWrite(Buzz, LOW);  if (StatoLed == 1) { digitalWrite(SpiaB,OFF);} ;
    delay(300);
    digitalWrite(Buzz, HIGH); if (StatoLed == 1) { digitalWrite(SpiaB,ON);} ;
    delay(300);
    digitalWrite(Buzz, LOW);  if (StatoLed == 1) { digitalWrite(SpiaB,OFF);} ;
    delay(300);
    digitalWrite(Buzz, HIGH); if (StatoLed == 1) { digitalWrite(SpiaB,ON);} ;
    delay(1000);
    digitalWrite(Buzz, LOW);  if (StatoLed == 1) { digitalWrite(SpiaB,OFF);} ;
}
