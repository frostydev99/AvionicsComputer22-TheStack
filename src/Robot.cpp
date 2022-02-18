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
 * Should take SPI/I2C/Serial objects as input parameters?
 */
bool Robot::systemInit(){

	//pinMode(LED_BUILTIN, OUTPUT);		// ! will conflict with CLK if using SPI !

	bool initIMU = false;


	initIMU = imu->init();

	if(!initIMU){
		Serial.println("ICM20948 does not respond");
	}
	else{
		Serial.println("ICM20948 is connected");
	}



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

	//zeroAllSensors();

}


void Robot::updateStateMachine(uint32_t timestamp){


//	imu->readSensorData();
//	imu->printVector(imu->getAccRawValues());


	// Send IMU data for the groundstation to be parsed and displayed
	uint8_t * timestampBytes = (uint8_t *) &timestamp;

	imu->readSensorData();

	//Data start bytes
	Serial.write(66); // B
	Serial.write(69); // E
	Serial.write(71); // G
	Serial.write(66); // B

	Serial.write(84); // T - Timestamp
	Serial.write(83); // S
	Serial.write(timestampBytes[3]);
	Serial.write(timestampBytes[2]);
	Serial.write(timestampBytes[1]);
	Serial.write(timestampBytes[0]);

	imu->printBuffer();

	// Data end bytes
	Serial.write(69); // E
	Serial.write(78); // N
	Serial.write(68); // D
	Serial.write(66); // B


}


void Robot::endStateMachine(){

}
