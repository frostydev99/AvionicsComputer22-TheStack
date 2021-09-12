#include "Arduino.h"

const uint32_t BAUD = 115200;



// The setup routine runs once when you press reset:
void setup() {

	Serial.begin(BAUD);		// issues sometime arise if this isn't done in setup

	pinMode(LED_BUILTIN, OUTPUT);

	Serial.println(F("Setup ran"));
}



// The loop routine runs over and over again forever:
void loop() {


	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	delay(1000);


}

