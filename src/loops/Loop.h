/*
 * loop.h
 * Created on: Sep 12, 2021
 * Author: Peter Dentch
 */

#ifndef SRC_LOOPS_LOOP_H_
#define SRC_LOOPS_LOOP_H_

#include "Arduino.h"


/*
 * Abstract class for routines which should be run periodically on the system
 * such as PID controller sampling or updating the state machine
 */
class Loop {

public:

	Loop(){}
//	virtual ~Loop(){}

	virtual void onStart(uint32_t timestamp);
	virtual void onLoop(uint32_t timestamp);
	virtual void onStop(uint32_t timestamp);

};



#endif /* SRC_LOOPS_LOOP_H_ */
