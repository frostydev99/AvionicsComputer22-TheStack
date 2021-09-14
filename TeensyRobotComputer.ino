/**
 * TeensyRobotComputer.ino
 * Created on: Sep 12, 2021
 * Author: Peter Dentch
 *
 * Main .ino program file
 * Code developed for Teensy 4.0
 *
 */

#include "Arduino.h"

#include "SystemManager.h"


SystemManager * sysMan;


// The setup routine runs once when you press reset:
void setup() {

	sysMan = new SystemManager();
	//Serial.begin(115200);
	//Serial.println(F("Serial debug connected"));

}


// The loop routine runs over and over again forever:
void loop() {

	sysMan->mainLoop();
	//delay(10);
	//Serial.println(millis());

}
