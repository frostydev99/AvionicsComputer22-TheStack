/*
 * Servo.h
 *
 *  Created on: May 25, 2022
 *      Author: Christian M. Schrader
 */

#ifndef SRC_PERIPHERALS_SERVO_H_
#define SRC_PERIPHERALS_SERVO_H_

#include "Arduino.h"

/*
 * A simple servo peripheral that incorporates both hardware and software limits.
 */
class Servo {
public:
	Servo(int pwmPin, int rangeLower, int rangeUpper, int limitLower, int limitUpper);
	bool actuate(int angle);
	int getAngle();

private:
	int pin;
	int currentAngle;
	int rangeLow;
	int rangeHigh;
	int limitLow;
	int limitHigh;
};

#endif /* SRC_PERIPHERALS_SERVO_H_ */
