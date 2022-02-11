/*
 * Robot.cpp
 * Created on: Sept 12, 2021
 * Author: Peter Dentch
 */

#include "Robot.h"

/*
* Constructor for the robot system object, should only be one instance, one main system per processor
*/
Robot::Robot(){};


/*
 * Init function for the system, should be run after instantiation
 * Should take SPI/I2C/Serial objects as imput parameters?
 */
bool Robot::systemInit(){

	//pinMode(LED_BUILTIN, OUTPUT);		// ! will conflict with CLK if using SPI !


	return true;

}

/*
 * Function to register all loops to the system looper. The looper must have the
 * total number of system loops predefined, TOTAL_LOOPS must equal the number of
 * registerLoop() calls in this function, see Constants.h
 * @param runningLooper is the looper instance of the system manager to call
 * for adding loops
 */
void Robot::registerAllLoops(Looper * runningLooper){

	runningLooper->registerLoop(robotLoop);

}


void Robot::zeroAllSensors(){

	//robotStateEstimator->reset(millis());

	//selfRighting->zeroSensors();

}


/*
 * Configuring robot subsystems for start of mission states sequence
 */
void Robot::beginStateMachine(){
	while(!Serial) {}
	  if(!imu->init()){
	    Serial.println("ICM20948 does not respond");
	  }
	  else{
	    Serial.println("ICM20948 is connected");
	  }
	Serial.println(F("STARTED ROBOT LOOP"));
	//zeroAllSensors();

}


void Robot::updateStateMachine(uint32_t timestamp){


	//digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	//Serial.println(millis());
	//	Serial.println(timestamp);
	imu->readSensorData();
	imu->printVector(imu->getAccRawValues());


}


void Robot::endStateMachine(){

}
