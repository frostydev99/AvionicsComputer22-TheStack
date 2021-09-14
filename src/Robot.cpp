/*
 * Robot.cpp
 * Created on: Sept 12, 2021
 * Author: Peter Dentch
 */

#include "Robot.h"

/*
* Constructor for the robot system object, should only be one instance, one robot per processor
* @param looper is the running looper of the system manager
*/
Robot::Robot(Looper * looper){
	runningLooper = looper;
	robotState = Waiting;
};


/*
 * Init function for the system, should be run after instantiation
 */
void Robot::systemInit(){

	runningLooper->registerLoop(robotLoop);		// robot system loop is registered first in order to be run first

	//selfRighting->registerEnabledLoops(runningLooper);
	//selfRighting->subsystemInit();


//	etc.


}


void Robot::zeroAllSensors(){

	//robotStateEstimator->reset(millis());

	//selfRighting->zeroSensors();

}


/*
 * Configuring robot subsystems for start of mission states sequence
 */
void Robot::beginStateMachine(){

	//Serial.println(F("START ROBOT LOOP"));
	//zeroAllSensors();

}


void Robot::updateStateMachine(){


//	runningLooper->printOutput();//TODO Only for debug, func should be private, lazyyy
//	Serial.println(driveTrain->getHeading());

	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	//Serial.println(millis());




}


void Robot::endStateMachine(){

}
