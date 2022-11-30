/*
 * Servo.cpp
 *
 *  Created on: May 25, 2022
 *      Author: Christian M. Schrader
 */

#include "Servo.h"

/**
 * Initializes servo peripheral
 * pwmPin: PWM Pin to control
 * rangeLower: Lower hardware rotational range of the servo in degrees.
 * rangeUpper: Upper hardware rotational range of the servo in degrees.
 * limitLower: Lower software rotational range of the servo in degrees.
 * limitUpper: Upper software rotational range of the servo in degrees.
 */
Servo::Servo(int pwmPin, int rangeLower, int rangeUpper, int limitLower, int limitUpper) {
	this->pin = pwmPin;
	this->currentAngle = 0;
	this->rangeLow = rangeLower;
	this->rangeHigh = rangeUpper;
	this->limitLow = limitLower;
	this->limitHigh = limitUpper;
}

bool Servo::actuate(int angle){
	bool ret = false;
	bool range = angle >= this->rangeLow && angle <= this->rangeHigh;
	bool limit = angle >= this->limitLow && angle <= this->limitHigh;
	if (range && limit) {
		ret = true;
		this->currentAngle = angle;
		analogWrite(this->pin, map(angle, this->rangeLow, this->rangeHigh, 0, 255));
	}
	return ret;
}


int Servo::getAngle() {
	return(this->currentAngle);
}
