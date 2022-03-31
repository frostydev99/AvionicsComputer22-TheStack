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

	//	Serial1.begin(9600);
	//	transceiver->init();
	//
	//	//transceiver->SetMode();
	//	transceiver->SetAddressH(0);
	//	transceiver->SetAddressL(0);
	//	transceiver->SetChannel(1);
	//	//transceiver->SetOptions();
	//
	//	transceiver->SetParityBit(0);	 		// SpeedParityBit
	//	transceiver->SetUARTBaudRate(3);		// 3 = 9600 baud
	//	transceiver->SetAirDataRate(4);			// 2 = B010 =  2.4kbps (default)
	//											// 4 = B100 = 9.6kbps
	//											// 5 = B101 = 19.2kbps
	//	transceiver->SetTransmissionMode(0);	// OptionTrans
	//	transceiver->SetPullupMode(1);			// OptionPullup
	//	transceiver->SetWORTIming(0);			// OptionWakeup
	//	transceiver->SetFECMode(1);				// OptionFEC
	//	transceiver->SetTransmitPower(0);		// default
	//
	//	transceiver->SaveParameters(PERMANENT);
	//	transceiver->PrintParameters();


	// Set up barometer
	baro->init();

	baro->setModeAltimeter();
	baro->setOverSample6ms();

	// Set up IMU
	imu->init();

	imu->setPlusMinus2000DPS();			// maximum angular rate measuring
	imu->setPlusMinus16Gs();			// maximum acceleration measuring


	//	delay(10);							// let sensors start up

	Serial.print("START UP STATE: IDLE");


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

	altitude = baro->getAltitude();
	prevAltitude = altitude;

}


void Robot::updateStateMachine(uint32_t timestamp){

	uint8_t * timestampBytes = (uint8_t *) &timestamp;

	imu->readSensorData();
	uint8_t * gyroAccelBytes = imu->getRawSensorRegisters();
	//	imu->printVector(imu->getAccRawValues());
	//Serial.println(imu->getTempRawValues());

	baro->readSensorData();
	uint32_t  altAndTemperature = baro->getPressureAndTempCombined();
	uint8_t * barometerBytes = (uint8_t *) &altAndTemperature;

	prevAltitude = altitude;
	rawAltitude = baro->getAltitude();

	uint8_t * timeBytes = (uint8_t *) &timestamp;

	// Filter
	altitude = 	rawAltitude + ALPHA * (prevAltitude - rawAltitude);
	uint8_t * altitudeBytes = (uint8_t *) &altitude;
	uint8_t * rawAltitudeBytes = (uint8_t *) &rawAltitude;
//	Serial.printf("Raw: %f, Filtered: %f \n", rawAltitude, altitude);
	Serial.write(66); //B
	Serial.write(69); //E
	Serial.write(71); //G
	Serial.write(66); //B


	Serial.write(65); //A
	Serial.write(76); //L
	Serial.write(84); //T

	Serial.write(altitudeBytes[3]);
	Serial.write(altitudeBytes[2]);
	Serial.write(altitudeBytes[1]);
	Serial.write(altitudeBytes[0]);

	Serial.write(65); //A
	Serial.write(67); //L
	Serial.write(88); //T

	Serial.write(rawAltitudeBytes[3]);
	Serial.write(rawAltitudeBytes[2]);
	Serial.write(rawAltitudeBytes[1]);
	Serial.write(rawAltitudeBytes[0]);

	Serial.write(84); //T
	Serial.write(83); //S
	Serial.write(80); //P

	Serial.write(timeBytes[3]);
	Serial.write(timeBytes[2]);
	Serial.write(timeBytes[1]);
	Serial.write(timeBytes[0]);


	Serial.write(69); //E
	Serial.write(78); //N
	Serial.write(68); //D
	Serial.write(66); //B

	temperature = baro->getTemperature();
	uint8_t * temperatureBytes = (uint8_t *) &temperature;
	//	Serial.println(temperature);

	// Main rocket state machine
//	switch (robotState) {
//
//	case BOTTOM_OF_HIGGINS:
//
//		if (altitude > altitudeThreshold) {
//			robotState = TOP_OF_HIGGINS;
//			Serial.printf("STATE CHANGE: From BOTTOM_OF_HIGGINS to TOP_OF_HIGGINS at %f", altitude);
//			Serial.println();
//		}
//
//		break;
//
//	case TOP_OF_HIGGINS:
//
//		if (altitude <= altitudeThreshold) {
//			robotState = BOTTOM_OF_HIGGINS;
//			Serial.printf("STATE CHANGE: From TOP_OF_HIGGINS to BOTTOM_OF_HIGGINS at %f", altitude);
//			Serial.println();
//		}
//
//		break;
//
//	case IDLE:
//
//		if (altitude > altitudeThreshold) {
//			robotState = TOP_OF_HIGGINS;
//			Serial.printf("STATE CHANGE: From IDLE to TOP_OF_HIGGINS at %f", altitude);
//			Serial.println();
//		}
//		else {
//			robotState = BOTTOM_OF_HIGGINS;
//			Serial.printf("STATE CHANGE: From IDLE to BOTTOM_OF_HIGGINS at %f", altitude);
//			Serial.println();
//		}
//
//		break;
//
//	default:
//
//		Serial.print("Code == broke");
//
//		break;
//
//	}

}


void Robot::endStateMachine(){

}
