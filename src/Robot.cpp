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


	DataPacket flightDataPacket;


	uint8_t * timestampBytes = (uint8_t *) &timestamp;

	flightDataPacket.count0 = timestampBytes[3];
	flightDataPacket.count1 = timestampBytes[2];
	flightDataPacket.count2 = timestampBytes[1];
	flightDataPacket.count3 = timestampBytes[0];

	uint16_t sensorValue = analogRead(A0); 			// read the input on analog pin 0
	float voltage = sensorValue * (3.3 / 1023.0); 	// Volts per count
	uint8_t * voltageBytes = (uint8_t *) &voltage;

	flightDataPacket.count4 = voltageBytes[3];
	flightDataPacket.count5 = voltageBytes[2];
	flightDataPacket.count6 = voltageBytes[1];
	flightDataPacket.count7 = voltageBytes[0];
	//Serial.println(voltage);

	dataLogger->setCurrentDataPacket(flightDataPacket);


	//Serial.println(timestamp);
	//uint8_t * timestampBytes = (uint8_t *) &timestamp;

	//imu->readSensorData();
	//uint8_t * gyroAccelBytes = imu->getRawSensorRegisters();

	//imu->printVector(imu->getAccRawValues());
	//Serial.println(imu->getTempRawValues());

	//baro->readSensorData();
	//uint32_t  altAndTemperature = baro->getPressureAndTempCombined();
	//uint8_t * barometerBytes = (uint8_t *) &altAndTemperature;

	//float altitude = baro->getAltitude();
	//uint8_t * altitudeBytes = (uint8_t *) &altitude;
	//Serial.println(altitude);

	//float temperature = baro->getTemperature();
	//uint8_t * temperatureBytes = (uint8_t *) &temperature;
	//Serial.println(temperature);







	 //RECEIVING
//	if(transceiver->available()) {
//
//		transceiver->GetStruct(&MyData, sizeof(MyData));
//
//
//		uint32_t altAndTemperature = 0;
//		uint8_t * barometerBytes = (uint8_t *) &altAndTemperature;
//
//		barometerBytes[0] = MyData.count4;
//		barometerBytes[1] = MyData.count5;
//		barometerBytes[2] = MyData.count6;
//		barometerBytes[3] = MyData.count7;
//
//		baro->setDataFromCombinedRaw(altAndTemperature);
//
//		float altitude = baro->getAltitude();
//		uint8_t * altitudeBytes = (uint8_t *) &altitude;
//		//Serial.println(altitude);
//
//		float temperature = baro->getTemperature();
//		uint8_t * temperatureBytes = (uint8_t *) &temperature;
//		//Serial.println(temperature);
//
//
//		//Data start bytes
//		Serial.write(66); // B
//		Serial.write(69); // E
//		Serial.write(71); // G
//		Serial.write(66); // B
//
//		Serial.write(84); // T - Timestamp
//		Serial.write(83); // S
//		Serial.write(80); // P
//		Serial.write(MyData.count0);
//		Serial.write(MyData.count1);
//		Serial.write(MyData.count2);
//		Serial.write(MyData.count3);
//
//		Serial.write(83); // S - State
//		Serial.write(84); // T
//		Serial.write(84); // T
//		Serial.write(0);  // state zero hardcode for now
//
//		Serial.write(65); // A - Altitute
//		Serial.write(76); // L
//		Serial.write(84); // T
//		Serial.write(altitudeBytes[3]);
//		Serial.write(altitudeBytes[2]);
//		Serial.write(altitudeBytes[1]);
//		Serial.write(altitudeBytes[0]);
//
//		Serial.write(84); // T - Temperature
//		Serial.write(77); // M
//		Serial.write(80); // P
//		Serial.write(temperatureBytes[3]);
//		Serial.write(temperatureBytes[2]);
//		Serial.write(temperatureBytes[1]);
//		Serial.write(temperatureBytes[0]);
//
//		// Accelerometer x-axis
//		Serial.write(65); // A
//		Serial.write(67); // C
//		Serial.write(88); // X
//		Serial.write(MyData.count8);
//		Serial.write(MyData.count9);
//
//		// Accelerometer y-axis
//		Serial.write(65); // A
//		Serial.write(67); // C
//		Serial.write(89); // Y
//		Serial.write(MyData.count10);
//		Serial.write(MyData.count11);
//
//		// Accelerometer z-axis
//		Serial.write(65); // A
//		Serial.write(67); // C
//		Serial.write(90); // Z
//		Serial.write(MyData.count12);
//		Serial.write(MyData.count13);
//
//		// Gyro x-axis
//		Serial.write(71); // G
//		Serial.write(89); // Y
//		Serial.write(88); // X
//		Serial.write(MyData.count14);
//		Serial.write(MyData.count15);
//
//		// Gyro y-axis
//		Serial.write(71); // G
//		Serial.write(89); // Y
//		Serial.write(89); // Y
//		Serial.write(MyData.count16);
//		Serial.write(MyData.count17);
//
//		// Gyro z-axis
//		Serial.write(71); // G
//		Serial.write(89); // Y
//		Serial.write(90); // Z
//		Serial.write(MyData.count18);
//		Serial.write(MyData.count19);
//
//		// Data end bytes
//		Serial.write(69); // E
//		Serial.write(78); // N
//		Serial.write(68); // D
//		Serial.write(66); // B
//
//
//
////		Serial.print(MyData.count0); Serial.print(F(", "));
////		Serial.print(MyData.count1); Serial.print(F(", "));
////		Serial.print(MyData.count2); Serial.print(F(", "));
////		Serial.print(MyData.count3); Serial.print(F(", "));
////		Serial.print(MyData.count4); Serial.print(F(", "));
////		Serial.print(MyData.count5); Serial.print(F(", "));
////		Serial.print(MyData.count6); Serial.print(F(", "));
////		Serial.print(MyData.count7); Serial.print(F(", "));
////		Serial.print(MyData.count8); Serial.print(F(", "));
////		Serial.print(MyData.count9); Serial.print(F(", "));
////		Serial.print(MyData.count10); Serial.print(F(", "));
////		Serial.print(MyData.count11); Serial.print(F(", "));
////		Serial.print(MyData.count12); Serial.print(F(", "));
////		Serial.print(MyData.count13); Serial.print(F(", "));
////		Serial.print(MyData.count14); Serial.print(F(", "));
////		Serial.print(MyData.count15); Serial.print(F(", "));
////		Serial.print(MyData.count16); Serial.print(F(", "));
////		Serial.print(MyData.count17); Serial.print(F(", "));
////		Serial.print(MyData.count18); Serial.print(F(", "));
////		Serial.print(MyData.count19);
////		Serial.println();
//	}



}


void Robot::endStateMachine(){

}
