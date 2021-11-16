/*
 * Looper.cpp
 * Created on: Dec 20, 2020
 * Author: Peter Dentch
 */

#include "Looper.h"

/*
 * Constructor for Looper object, should be only one instance
 */
Looper::Looper(){}


/*
 * Function to add a Loop object to the Looper's list of pointers so they can be executed accordingly
 * @param systemLoop is the loop to add to the list
 */
void Looper::registerLoop(Loop * systemLoop){

	loops_[loopIndex_] = systemLoop;		// enabled Looper needs loops successfully registered to run
	loopIndex_++;

}


/*
 * Function to check for system loops which failed to register and have null pointers in the loops_ list.
 * Calling null loops will result in runtime errors
 * @return 1 + the first position in loops_ to have found a null pointer
 */
uint8_t Looper::checkLoopList(){

	uint8_t nullPtrPos = 0;						// are any Loops in loops_ non-existant?

	for(size_t i = 0; i < TOTAL_NUM_LOOPS; i++) {

		if(loops_[i] == NULL){

			nullPtrPos = i + 1; // + 1 so that output can be checked as a bool (false = 0, true = !false)

		}
	}

	return nullPtrPos;

}


/*
 * Function to start all registered loops, executes all onStart()s
 */
bool Looper::startLoops(){

	bool allLoopsRegistered = (checkLoopList() == 0);		// check loops_ for any null pointers

	// Check if the looper isn't already running and all loops have been successfully registered
	if(!running_ && allLoopsRegistered) {

		uint32_t now = millis();

		for(size_t i = 0; i < TOTAL_NUM_LOOPS; i++) {

			loops_[i]->onStart(now);
		}

		timestamp_ = millis();
		dt_ = timestamp_ - now;

		running_ = true;

		loopTimer->reset();			// allegedly helps prevent odd timestamp readings in onLoop()

		return true;


	} else {
		//Serial.println(F("! Loops not started !"));
		return false;
	}

}


/*
 * Function to be iterated over and over, runs all loops of the system by executing onLoop()s
 */
bool Looper::runLoops(){

	// Check if the looper is running and the timer has passed it's interval
	if(running_ && loopTimer->check() == 1){

		uint32_t now = millis();

		for(size_t i = 0; i < TOTAL_NUM_LOOPS; i++) {

			loops_[i]->onLoop(now);
		}

		dt_ = now - timestamp_;
		timestamp_ = now;

		return true;

	} else {
		//Serial.println(F("! Loops not run !"));
		return false;
	}

}


/*
 * Function to stop all registered loops, executes all onStop()s
 */
bool Looper::stopLoops(){

	if(running_){

		//Serial.println(F("Loops stopped"));
		//Serial.print(F("Looper timestamp: ")); Serial.println(timestamp_);
		printOutput();

		running_ = false;

		uint32_t now = millis();

		for(size_t i = 0; i < TOTAL_NUM_LOOPS; i++) {

			loops_[i]->onStop(now);
		}



	}
	return false;
}


/*
 * Debug printing function for the Looper object, outputs looper execution properties (frequency, delta t, time stamp)
 */
void Looper::printOutput(){

#ifdef USE_SERIAL
//	Serial.print(F("Looper timestamp: ")); Serial.println(timestamp_);
//	Serial.print(F("Looper dt: ")); Serial.println(dt_);

	Serial.print(F("Took ")); Serial.print((float)dt_ / 1000, 3); Serial.println(F(" ms to run"));
	//Serial.print(F("Looper frequency: "));
	Serial.print( 1 / ((float)dt_ / 1000000) ); Serial.println(F(" Hz"));
#endif

}



