/*
 * MetroTimer.h
 *
 * Created on: Nov 15, 2021
 * Author: Peter Dentch
 */

#ifndef SRC_PERIPHERALS_METROTIMER_H_
#define SRC_PERIPHERALS_METROTIMER_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/*
 * A timer object meant to be a copy of the Metro timer library for Arduino.
 * People keep experiencing issues compiling because they're unable to successfully
 * include the Metro library for whatever reason so this was created to use instead
 */
class MetroTimer {

public:

	MetroTimer(unsigned long interval_millis);
	MetroTimer(unsigned long interval_millis, uint8_t autoreset);
	void interval(unsigned long interval_millis);
	char check();
	void reset();

private:

	uint8_t autoreset;
	unsigned long previous_millis;
	unsigned long interval_millis;

};



#endif /* SRC_PERIPHERALS_METROTIMER_H_ */
