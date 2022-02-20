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


//	delay(10);							// let sensors start up

	Serial1.begin(9600);
	transceiver->init();

	//transceiver->SetMode();
	transceiver->SetAddressH(0);
	transceiver->SetAddressL(0);
	transceiver->SetChannel(1);
	//transceiver->SetOptions();

	transceiver->SetParityBit(0);	 		// SpeedParityBit
	transceiver->SetUARTBaudRate(3);		// 3 = 9600 baud
	transceiver->SetAirDataRate(4);			// 2 = B010 =  2.4kbps (default)
											// 4 = B100 = 9.6kbps
											// 5 = B101 = 19.2kbps
	transceiver->SetTransmissionMode(0);	// OptionTrans
	transceiver->SetPullupMode(1);			// OptionPullup
	transceiver->SetWORTIming(0);			// OptionWakeup
	transceiver->SetFECMode(1);				// OptionFEC
	transceiver->SetTransmitPower(0);		// default

	transceiver->SaveParameters(PERMANENT);
	transceiver->PrintParameters();



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

	uint8_t * timestampBytes = (uint8_t *) &timestamp;

	imu->readSensorData();
	uint8_t * gyroAccelBytes = imu->getRawSensorRegisters();
	imu->printVector(imu->getAccRawValues());
	//Serial.println(imu->getTempRawValues());

	baro->readSensorData();
	uint32_t  altAndTemperature = baro->getPressureAndTempCombined();
	uint8_t * barometerBytes = (uint8_t *) &altAndTemperature;

	float altitude = baro->getPressure();
	uint8_t * altitudeBytes = (uint8_t *) &altitude;
	//Serial.println(altitude);

	float temperature = baro->getTemperature();
	uint8_t * temperatureBytes = (uint8_t *) &temperature;
	//Serial.println(temperature);



	// SENDING

//	MyData.count0 = timestampBytes[3];
//	MyData.count1 = timestampBytes[2];
//	MyData.count2 = timestampBytes[1];
//	MyData.count3 = timestampBytes[0];
//
//	MyData.count4 = barometerBytes[0];
//	MyData.count5 = barometerBytes[1];
//	MyData.count6 = barometerBytes[2];
//	MyData.count7 = barometerBytes[3];
//
//	MyData.count8 = gyroAccelBytes[0];
//	MyData.count9 = gyroAccelBytes[1];
//	MyData.count10 = gyroAccelBytes[2];
//	MyData.count11 = gyroAccelBytes[3];
//	MyData.count12 = gyroAccelBytes[4];
//	MyData.count13 = gyroAccelBytes[5];
//	MyData.count14 = gyroAccelBytes[6];
//	MyData.count15 = gyroAccelBytes[7];
//	MyData.count16 = gyroAccelBytes[8];
//	MyData.count17 = gyroAccelBytes[9];
//	MyData.count18 = gyroAccelBytes[10];
//	MyData.count19 = gyroAccelBytes[11];
//
//
//	transceiver->SendStruct(&MyData, sizeof(MyData));
//	Serial.println(MyData.count2);



    //Data start bytes
//    Serial.write(66); // B
//    Serial.write(69); // E
//    Serial.write(71); // G
//    Serial.write(66); // B
//
//    Serial.write(84); // T - Timestamp
//    Serial.write(83); // S
//    Serial.write(80); // P
//    Serial.write(timestampBytes[3]);
//    Serial.write(timestampBytes[2]);
//    Serial.write(timestampBytes[1]);
//    Serial.write(timestampBytes[0]);
//
//    Serial.write(83); // S - State
//    Serial.write(84); // T
//    Serial.write(84); // T
//    Serial.write(0);  // state zero hardcode for now
//
//    Serial.write(65); // A - Altitute
//    Serial.write(76); // L
//    Serial.write(84); // T
//    Serial.write(altitudeBytes[3]);
//    Serial.write(altitudeBytes[2]);
//    Serial.write(altitudeBytes[1]);
//    Serial.write(altitudeBytes[0]);
//
//    Serial.write(84); // T - Temperature
//    Serial.write(77); // M
//    Serial.write(80); // P
//    Serial.write(temperatureBytes[3]);
//    Serial.write(temperatureBytes[2]);
//    Serial.write(temperatureBytes[1]);
//    Serial.write(temperatureBytes[0]);
//
//    // Accelerometer x-axis
//	Serial.write(65); // A
//	Serial.write(67); // C
//	Serial.write(88); // X
//	Serial.write(gyroAccelBytes[0]);
//	Serial.write(gyroAccelBytes[1]);
//
//	// Accelerometer y-axis
//	Serial.write(65); // A
//	Serial.write(67); // C
//	Serial.write(89); // Y
//	Serial.write(gyroAccelBytes[2]);
//	Serial.write(gyroAccelBytes[3]);
//
//	// Accelerometer z-axis
//	Serial.write(65); // A
//	Serial.write(67); // C
//	Serial.write(90); // Z
//	Serial.write(gyroAccelBytes[4]);
//	Serial.write(gyroAccelBytes[5]);
//
//	// Gyro x-axis
//	Serial.write(71); // G
//	Serial.write(89); // Y
//	Serial.write(88); // X
//	Serial.write(gyroAccelBytes[6]);
//	Serial.write(gyroAccelBytes[7]);
//
//	// Gyro y-axis
//	Serial.write(71); // G
//	Serial.write(89); // Y
//	Serial.write(89); // Y
//	Serial.write(gyroAccelBytes[8]);
//	Serial.write(gyroAccelBytes[9]);
//
//	// Gyro z-axis
//	Serial.write(71); // G
//	Serial.write(89); // Y
//	Serial.write(90); // Z
//	Serial.write(gyroAccelBytes[10]);
//	Serial.write(gyroAccelBytes[11]);
//
//    // Data end bytes
//    Serial.write(69); // E
//    Serial.write(78); // N
//    Serial.write(68); // D
//    Serial.write(66); // B






	// RECEIVING
//	if(transceiver->available()) {
//
//		transceiver->GetStruct(&MyData, sizeof(MyData));



		//Serial.print(timestamp); Serial.print(F(": "));
		//Serial.println(MyData.Count);

//		Serial.print(MyData.count0); Serial.print(F(", "));
//		Serial.print(MyData.count1); Serial.print(F(", "));
//		Serial.print(MyData.count2); Serial.print(F(", "));
//		Serial.print(MyData.count3); Serial.print(F(", "));
//		Serial.print(MyData.count4); Serial.print(F(", "));
//		Serial.print(MyData.count5); Serial.print(F(", "));
//		Serial.print(MyData.count6); Serial.print(F(", "));
//		Serial.print(MyData.count7); Serial.print(F(", "));
//		Serial.print(MyData.count8); Serial.print(F(", "));
//		Serial.print(MyData.count9); Serial.print(F(", "));
//		Serial.print(MyData.count10); Serial.print(F(", "));
//		Serial.print(MyData.count11); Serial.print(F(", "));
//		Serial.print(MyData.count12); Serial.print(F(", "));
//		Serial.print(MyData.count13); Serial.print(F(", "));
//		Serial.print(MyData.count14); Serial.print(F(", "));
//		Serial.print(MyData.count15); Serial.print(F(", "));
//		Serial.print(MyData.count16); Serial.print(F(", "));
//		Serial.print(MyData.count17); Serial.print(F(", "));
//		Serial.print(MyData.count18); Serial.print(F(", "));
//		Serial.print(MyData.count19);
//		Serial.println();
//	}



}


void Robot::endStateMachine(){

}
