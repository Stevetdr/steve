// https://www.arduino.cc/en/Reference/attachInterrupt
// per approfondire    http://gammon.com.au/interrupts

const byte ledPin = 13;
const byte interruptPin = 2;	// per nano puo' essere solo 2 o 3
volatile byte state = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  //                                       pin        routine  stato
  // lo stato puo' essere CHANGE: se il pin cambia di valore da Hi a LO o da LO a HI
  //                      LOW: se lo stato e' LOW trig dell'int
  //                      RISING: quando il pin passa da low a high
  //                      FALLING: quando il pin passa da high a low
  //					  HIGH: da chiarire se applicabile su nano
  // routine: quando succede interrupt, salta alla routine "blink"
}

void loop() {
  digitalWrite(ledPin, state);
}

void blink() {
  state = !state;
}