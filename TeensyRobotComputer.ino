/**
 * TeensyRobotComputer.ino
 * Created on: Sep 12, 2021
 * Author: Peter Dentch
 *
 * Main .ino program file
 * Code developed for Arduino
 * and Teensy 4.0
 *
 */

#include "Arduino.h"

#include "SystemManager.h"


SystemManager * sysMan = new SystemManager();


// The setup routine runs once when you press reset:
void setup() {

	sysMan->mainSetup();

}


// The loop routine runs over and over again forever:
void loop() {

	sysMan->mainLoop();

}
