/*
 * DataLogger.cpp
 *
 * Created on: Mar 26, 2022
 * Author: Peter Dentch
 */

#include "DataLogger.h"

/*
* Constructor for the DataLogger subsystem object
*/
DataLogger::DataLogger(){};


/*
 * Init function for the system, should be run after instantiation
 */
bool DataLogger::subsystemInit(){

	// Flash memory
	bool flashInit = false;
	flashInit = flashMemoryInit();

	// Transceiver radio
	bool radioInit = false;
	radioInit = transcieverInit();


	//delay(10);

	return flashInit && radioInit;

}


/*
 *
 */
bool DataLogger::transcieverInit() {

	Serial1.begin(9600);

	bool initSuccess = false;
	//initSuccess = transceiver->init(1);

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

	return initSuccess;

}


/*
 *
 */
bool DataLogger::flashMemoryInit() {

	SerialFlash.begin(PIN_CS);

    //Function immediately checks if the file already exists, else does nothing!
    bool fileCreation = SerialFlash.createErasable(fileName, circularBufferSize);

    //Serial.print("Circular buffer size: "); Serial.println(circularBufferSize);
    //Serial.print("Sensor data size: "); Serial.println(dataPacketSize);

    if(fileCreation) {
      Serial.println("File created successfully!");
    } else {
      Serial.println("File not created/File Already Exists");
    }

    file = SerialFlash.open(fileName);

    //getFlashAddress gives the file address in the memory, blocks of 2^16 for using createErasable
    Serial.print("Address: "); Serial.println(file.getFlashAddress());	// should be 65536
    Serial.print("File Position: ");Serial.println(file.position());


	return fileCreation;

}


/*
 *
 */
bool DataLogger::timeToTransmit() {
	return transmitTimer.check() == 1;			//check if the timer has passed it's interval
}


/*
 *
 */
bool DataLogger::transmitTelemetry() {

	bool sendSuccess = false;

//	dataPacket.count0 = timestampBytes[3];
//	dataPacket.count1 = timestampBytes[2];
//	dataPacket.count2 = timestampBytes[1];
//	dataPacket.count3 = timestampBytes[0];
//
//	dataPacket.count4 = barometerBytes[0];
//	dataPacket.count5 = barometerBytes[1];
//	dataPacket.count6 = barometerBytes[2];
//	dataPacket.count7 = barometerBytes[3];
//
//	dataPacket.count8 = gyroAccelBytes[0];
//	dataPacket.count9 = gyroAccelBytes[1];
//	dataPacket.count10 = gyroAccelBytes[2];
//	dataPacket.count11 = gyroAccelBytes[3];
//	dataPacket.count12 = gyroAccelBytes[4];
//	dataPacket.count13 = gyroAccelBytes[5];
//	dataPacket.count14 = gyroAccelBytes[6];
//	dataPacket.count15 = gyroAccelBytes[7];
//	dataPacket.count16 = gyroAccelBytes[8];
//	dataPacket.count17 = gyroAccelBytes[9];
//	dataPacket.count18 = gyroAccelBytes[10];
//	dataPacket.count19 = gyroAccelBytes[11];

	sendSuccess = transceiver->SendStruct(&dataPacket, sizeof(dataPacket));

	return sendSuccess;

}


/*
 * Zero drive train
 */
void DataLogger::zeroSensors(){

}

/*
 * Register data logger loop
 */
void DataLogger::registerLoops(Looper * runningLooper) {

	runningLooper->registerLoop(loggerLoop);
}

/*
 * Print output of data logger
 */
void DataLogger::printOutput() {
	Serial.println(F("TEST"));
}



