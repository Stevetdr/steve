const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin =  13;    // the number of the LED pin

// variables will change:
volatile int buttonState = 0;      // variable for reading the pushbutton status

void setup() {
	Serial.begin(9600);

	pinMode(ledPin, OUTPUT);				// initialize the LED pin as an output:
	pinMode(buttonPin, INPUT);				// initialize the pushbutton pin as an input:
	attachInterrupt(0, pin_ISR, CHANGE);	// Attach an interrupt to the ISR vector
}

void loop() {
	// Nothing here!
	while(1) {
		Serial.println("Normal Working 1");
		delay(1000);
		Serial.println("Normal Working 2");
		delay(1000);
	}
}

void pin_ISR() {
	buttonState = digitalRead(buttonPin);
	digitalWrite(ledPin, buttonState);
	Serial.println("INTERRUPT C A L L E D");
}