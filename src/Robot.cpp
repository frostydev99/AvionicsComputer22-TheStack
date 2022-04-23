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


	// Set up barometer
	baro->init();

	baro->setModeAltimeter();
	baro->setOverSample6ms();

	// Set up IMU
	imu->init();

	imu->setPlusMinus2000DPS();			// maximum angular rate measuring
	imu->setPlusMinus16Gs();			// maximum acceleration measuring


	// Datalogger
	dataLogger->subsystemInit();



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

	dataLogger->registerLoops(runningLooper);

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

	// Read sensors
	baro->readSensorData();
	imu->readSensorData();

	// Build a rocket telemetry data packet using sensor data
	packet.setTimestamp(timestamp);
	packet.setState(0);						// state zero hardcode for now

	packet.setAltitude(9999.99);
	packet.setTemperature(-4.5);
	packet.setAltAndTempCombined(baro->getPressureAndTempCombined());

	packet.setAccelX(imu->getRawAccelX());
	packet.setAccelY(imu->getRawAccelY());
	packet.setAccelZ(imu->getRawAccelZ());
	packet.setGyroX(imu->getRawGyroX());
	packet.setGyroY(imu->getRawGyroY());
	packet.setGyroZ(imu->getRawGyroZ());

	packet.updateToTelemPacket();			// for transmitter to use


	//Serial.println(packet.getAltitude());
	//Serial.println(packet.getTemperature());

	// Update the packet for the dataLogger to transmit
	dataLogger->setCurrentDataPacket(packet.getTelemRocketPacketPtr(), 20);


	// Testing copying data from one packet object to another
	testPacket.setRocketTelemPacket(packet.getTelemRocketPacketPtr());
	testPacket.updateFromTelemPacket();		// for receiver to use

	//Serial.println(testPacket.getTemperature());


}


void Robot::endStateMachine(){

}
