/*
 * Looper.h
 * Created on: Dec 20, 2020
 * Author: Peter Dentch
 */

#ifndef SRC_LOOPS_LOOPER_H_
#define SRC_LOOPS_LOOPER_H_

#include "Arduino.h"

//#include <Metro.h>						// MetroTimer was created as a local copy of Metro.h
#include "../peripherals/MetroTimer.h"

#include "../../Constants.h"
#include "Loop.h"


#define LIST_SIZE TOTAL_LOOPS
#define DT 	 	  LOOPER_PERIOD


/*
 * Class for handling all loop functionality for the system, Loop objects (systems/subsystems) are
 * registered to this handler to be executed together at a set iteration time step.
 * Every system with a loop runs onStart when began, onStop when halted, and onLoop periodically
 * when this object is called by the main system
 */
class Looper {

private:

	const uint32_t PERIOD = DT;								// period for all fast loops of system
	MetroTimer * loopTimer = new MetroTimer(PERIOD);		// passed to timer in milliseconds

	const size_t TOTAL_NUM_LOOPS = LIST_SIZE;
	Loop * loops_[LIST_SIZE] = {};				// list of all registered loops objects running, their pointers
	uint8_t loopIndex_ = 0;						// index in loops_ for next registered loop

	bool running_ = false;						// is the Looper enabled and running?
	uint32_t timestamp_ = 0;					// last millis() timestamp of system loop execution
	uint32_t dt_ = 1;							// keeping track of the loop execution period


public:

	Looper();
//	~Looper(){}

    void registerLoop(Loop * systemLoop);
    uint8_t checkLoopList();

    bool startLoops();
    bool runLoops();
	bool stopLoops();

	void printOutput();

};


#endif /* SRC_LOOPS_LOOPER_H_ */
