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


	imu->setGyroScale(imu->getPlusMinus2000DPS());
	imu->setAccScale(imu->getPlusMinus8Gs());


	baro->init();

	baro->setModeAltimeter();
	baro->setOverSampleRate(0);

	delay(100);							// let barometer start up




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
//	imu->printVector(imu->getGyroRawValues());
	//Serial.println(imu->getTempRawValues());


	baro->readSensorData();

	float altitude = baro->getPressure();
	uint8_t * altitudeBytes = (uint8_t *) &altitude;
	//Serial.println(altitude);

	float temperature = baro->getTemperature();
	uint8_t * temperatureBytes = (uint8_t *) &temperature;
	//Serial.println(temperature);

	uint8_t * timestampBytes = (uint8_t *) &timestamp;


    //Data start bytes
    Serial.write(66); // B
    Serial.write(69); // E
    Serial.write(71); // G
    Serial.write(66); // B

    Serial.write(84); // T - Timestamp
    Serial.write(83); // S
    Serial.write(80); // P
    Serial.write(timestampBytes[3]);
    Serial.write(timestampBytes[2]);
    Serial.write(timestampBytes[1]);
    Serial.write(timestampBytes[0]);

    Serial.write(65); // A - Altitute
    Serial.write(76); // L
    Serial.write(84); // T
    Serial.write(altitudeBytes[3]);
    Serial.write(altitudeBytes[2]);
    Serial.write(altitudeBytes[1]);
    Serial.write(altitudeBytes[0]);

    Serial.write(84); // T - Temperature
    Serial.write(77); // M
    Serial.write(80); // P
    Serial.write(temperatureBytes[3]);
    Serial.write(temperatureBytes[2]);
    Serial.write(temperatureBytes[1]);
    Serial.write(temperatureBytes[0]);

    // Data end bytes
    Serial.write(69); // E
    Serial.write(78); // N
    Serial.write(68); // D
    Serial.write(66); // B




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
	Serial.write(80); // P
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
	baro->readSensorData();

	float altitude = baro->getPressure();
	uint8_t * altitudeBytes = (uint8_t *) &altitude;
	//Serial.println(altitude);

	float temperature = baro->getTemperature();
	uint8_t * temperatureBytes = (uint8_t *) &temperature;
	//Serial.println(temperature);

	uint8_t * timestampBytes = (uint8_t *) &timestamp;


    //Data start bytes
    Serial.write(66); // B
    Serial.write(69); // E
    Serial.write(71); // G
    Serial.write(66); // B

    Serial.write(84); // T - Timestamp
    Serial.write(83); // S
    Serial.write(80); // P
    Serial.write(timestampBytes[3]);
    Serial.write(timestampBytes[2]);
    Serial.write(timestampBytes[1]);
    Serial.write(timestampBytes[0]);

    Serial.write(65); // A - Altitute
    Serial.write(76); // L
    Serial.write(84); // T
    Serial.write(altitudeBytes[3]);
    Serial.write(altitudeBytes[2]);
    Serial.write(altitudeBytes[1]);
    Serial.write(altitudeBytes[0]);

    Serial.write(84); // T - Temperature
    Serial.write(77); // M
    Serial.write(80); // P
    Serial.write(temperatureBytes[3]);
    Serial.write(temperatureBytes[2]);
    Serial.write(temperatureBytes[1]);
    Serial.write(temperatureBytes[0]);

    // Data end bytes
    Serial.write(69); // E
    Serial.write(78); // N
    Serial.write(68); // D
    Serial.write(66); // B




}


void Robot::endStateMachine(){

}
