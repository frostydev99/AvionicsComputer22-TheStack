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
	initSuccess = transceiver->init(1);

	//transceiver->SetMode();
	transceiver->SetAddressH(0);
	transceiver->SetAddressL(0);
	transceiver->SetChannel(17);			// 55 = 917MHz for E32-900T20D
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
	transceiver->SetTransmitPower(3);		// 1W (0) default, 0.1W is 3

	transceiver->SaveParameters(PERMANENT);
	transceiver->PrintParameters();

	return initSuccess;

}


/*
 *
 */
bool DataLogger::flashMemoryInit() {

	SerialFlash.begin(PIN_CS);	//TODO check bool

    //Function immediately checks if the file already exists, else does nothing!
    bool fileCreation = SerialFlash.createErasable(bufferFileName, circularBufferSize);

    //Serial.print("Circular buffer size: "); Serial.println(circularBufferSize);
    //Serial.print("Sensor data size: "); Serial.println(dataPacketSize);

    if(fileCreation) {
      Serial.println("File created successfully!");
    } else {
      Serial.println("File not created/File Already Exists");
    }

    bufferFile = SerialFlash.open(bufferFileName);

    //getFlashAddress gives the file address in the memory, blocks of 2^16 for using createErasable
    Serial.print("Address: "); Serial.println(bufferFile.getFlashAddress());	// should be 65536
    Serial.print("File Position: ");Serial.println(bufferFile.position());

    Serial.print("File Size: "); Serial.println(bufferFile.size());


	return fileCreation;

}


/*
 *	Function for locating the
 */
void DataLogger::locateCircBufferAddresses() {

	// First: find the first block that has yet to be fully populated with data

	uint16_t totalBlocks = circularBufferSize / SerialFlash.blockSize();
	//Serial.print("Total blocks: "); Serial.println(totalBlocks);

	uint32_t blockToWriteAddr = 0;		// the first free block (chuck of 2^16 bytes)

	bool foundErasedBlock = false;

	//looping through the buffer
	for(uint16_t i = 0; i < totalBlocks; i++) {

		uint32_t blockAddress = i * SerialFlash.blockSize();
		uint32_t lastByteAddr = blockAddress + (SerialFlash.blockSize() - 1);

		uint8_t lastByteValue = 0;
		bufferFile.seek(lastByteAddr);
		bufferFile.read(&lastByteValue, 1);

		if(lastByteValue == 255){				// found first erased block in the buffer

			foundErasedBlock = true;
			blockToWriteAddr = blockAddress;
			break;
		}

	}

	// Must handle case if no free block is found (system powered off before erase complete)
	if(foundErasedBlock){
		Serial.print("First free block is at: "); Serial.println(blockToWriteAddr);
	} else {
		Serial.println(F("FAILED TO FIND ERASED BLOCK"));
	}


	// Second: find the first byte address in the block to write new data

	uint32_t byteToWriteAddr = 0;			// the first free byte in the block

	// looping backwards through the block
	for(uint32_t i = SerialFlash.blockSize(); i > 0; i--) {

		uint32_t byteAddress = blockToWriteAddr + (i - 1);
		uint8_t byteValue = 255;
		bufferFile.seek(byteAddress);
		bufferFile.read(&byteValue, 1);

		if(byteValue != 255){				// found first erased byte in the block

			endDataAddress = byteAddress;
			byteToWriteAddr = byteAddress + 1;		// the next address is blank, write to it

			break;
		}

	}

	//Serial.print("First free byte is at: "); Serial.println(byteToWriteAddr);

	// Third: find the next block with data in it

	// The first block to search for valid data (start 1 block after the current block)
	//uint32_t begBlockAddr = blockToWriteAddr + SerialFlash.blockSize();

	//looping through the remainder of the buffer
	for(uint16_t i = 0; i < totalBlocks; i++) {

		//uint32_t blockAddress = begBlockAddr + (i * SerialFlash.blockSize());
		uint32_t blockAddress = i * SerialFlash.blockSize();

		// Wrap around if out of bounds
		if(blockAddress > circularBufferSize){
			blockAddress = 0;
		}

		uint8_t byteValue = 255;
		bufferFile.seek(blockAddress);
		bufferFile.read(&byteValue, 1);

		//Serial.print("First byte is: "); Serial.println(byteValue);

		if(byteValue != 255){						// found data in first address of block

			begDataAddress = blockAddress;

			break;
		}


	}

	// Move to the byte address where data should resume being written to the buffer
	bufferFile.seek(byteToWriteAddr);

}


/*
 *
 * @return true if data written, false if block needs to be erased
 */
bool DataLogger::updateCircBuffer() {

	uint32_t currentAddress = bufferFile.position();
	Serial.print("Current address: "); Serial.println(currentAddress);

	// First update buffer end position

	// Check if the current buffer position is at the end of the buffer
	if(currentAddress >= circularBufferSize) {

		// Wrap around if out of bounds
		endDataAddress = 0;
		bufferFile.seek(0);			// set the current address for writing back to the start
	} else {

		// otherwise just update to the next position to write
		endDataAddress = currentAddress;
	}

	// Then check if the buffer position has reached the begDataAddress position
	// in which case the current block must be erased and begDataAddress set to the
	// start of the next block
	if(endDataAddress == begDataAddress){

		// Erase block
		Serial.print("Erase block: "); Serial.println(begDataAddress);

		uint32_t fileAddress = bufferFile.getFlashAddress();    // should be 0x00FFFF (2^16)
		SerialFlash.eraseBlock(fileAddress + begDataAddress);	// offset by file location

		// Update new beginning address
		begDataAddress = begDataAddress + SerialFlash.blockSize();

		// Wrap around if out of bounds
		if(begDataAddress >= circularBufferSize){

			begDataAddress = 0;
		}

		// Don't proceed to write if erase block command ran
		//setState(DATALOGGER_ERASE_BUFFER);
		//return false;

	}

	// Don't proceed to write if chip is busy (erase block command ran)
	if(!SerialFlash.ready()){
		return false;
	}


	uint32_t fileWriteLen = bufferFile.write(currentDataPacket, dataPacketSize);
	//Serial.print("Length of data written: "); Serial.println(fileWriteLen);
	//Serial.print("Current address: "); Serial.println(currentAddress);

//	Serial.print("Buffer begin addr: "); Serial.println(begDataAddress);
//	Serial.print("Buffer end addr: "); Serial.println(endDataAddress);


	return true;

}


/*
 *
 */
bool DataLogger::readCircBuffer() {

	// If there's a next data packet to read in the file, read and print it
	if(bufferFile.available() > 0){

		uint32_t currentAddress = bufferFile.position();
		Serial.print(currentAddress); Serial.print(", ");
		//Serial.print("Current address: "); Serial.println(currentAddress);

		uint8_t buffer[dataPacketSize];             // temporary data buffer for reading

		// Read the memory block
		uint32_t fileReadLen = bufferFile.read(buffer, dataPacketSize);
		//Serial.print("Length of data read: "); Serial.println(fileReadLen);

		//Serial.println("Whole buffer of data");
		//for(uint8_t i = 0; i < dataPacketSize; i++) {
		//	Serial.println(buffer[i]);
		//}


		setCurrentDataPacket(buffer, dataPacketSize);

		//printRawDataToSerialMonitor(bufferPacket);
//		printPacketToSerialMonitor(bufferPacket);
//		printPacketToGroundstation(bufferPacket);


		return true;

	} else {
		return false;
	}

}


/*
 *
 */
bool DataLogger::addPacketToSmallBuffer() {

	Serial.print("INDEX: "); Serial.println(smallBufferIndex);

	uint8_t* packetPtr = &dataPacketSmallBuffer[smallBufferIndex][0];
	memcpy(currentDataPacket, packetPtr, dataPacketSize);

	// Wrap around if out of bounds
	uint8_t smallBufferPacketNum = sizeof(dataPacketSmallBuffer) / dataPacketSize;

	if(smallBufferIndex >= smallBufferPacketNum - 1){
		smallBufferIndex = 0;
		return true;
	} else {
		smallBufferIndex++;
		return false;
	}

}


/*
 *
 */
void DataLogger::writeSmallBufferToCircBuffer() {


	for(uint8_t i = 0; i < smallBufferIndex; i++){

		uint8_t* packetPtr = &dataPacketSmallBuffer[i][0];

		uint32_t fileWriteLen = bufferFile.write(packetPtr, dataPacketSize);

		uint32_t currentAddress = bufferFile.position();
		endDataAddress = currentAddress;

	}

	smallBufferIndex = 0;				// reset to start of buffer after copying it

}



/*
 *
 */
bool DataLogger::timeToTransmit() {

	if(timer > 10){
		timer = 0;
		return true;
	} else {
		timer++;
		return false;
	}


	//return transmitTimer.check() == 1;			//check if the timer has passed it's interval
}


/*
 *
 */
bool DataLogger::transmitTelemetry() {

	bool sendSuccess = false;


	uint16_t dataPacketSizeToTransmit = 20;

	sendSuccess = transceiver->SendStruct(currentDataPacket, dataPacketSizeToTransmit);

	return sendSuccess;

}


/*
 *
 */
void DataLogger::receiveTelemetry() {

	if(transceiver->available()) {

		uint16_t telemDataPacketSize = 20;

		transceiver->GetStruct(&currentDataPacket, telemDataPacketSize);

		// Send packet to groundstation on good recieve
		printCurrentPacketToGroundstation();

	}

}


/*
 *
 */
void DataLogger::setState(DataLoggerState state) {
	loggerState = state;
}


/*
 * Set the currentDataPacket of the DataLogger
 * @param packet is the pointer to the start of the packet with data to copy
 * @param packetSize is the amount of data to copy to the currentDataPacket
 */
void DataLogger::setCurrentDataPacket(const void* packet, uint8_t packetSize) {
	memcpy(currentDataPacket, packet, packetSize);
}


/*
 * Get the currentDataPacket of the DataLogger
 * @param packet is the pointer to the start of the packet to copy data into
 * @param packetSize is the amount of data to copy
 */
void DataLogger::getCurrentDataPacket(void* packet, uint8_t packetSize) {
	memcpy(packet, currentDataPacket, packetSize);
}


/*
 * BIG ENDIAN
 */
void DataLogger::printCurrentPacketToGroundstation() {

	// Pass the data in the currentDataPacket of the dataLogger to a RocketDataPacket object
	RocketDataPacket packet;
	packet.setRocketTelemPacket(currentDataPacket);
	packet.updateFromTelemPacket();

	uint32_t timestamp = packet.getTimestamp();
	uint8_t* timestampBytes = (uint8_t*) &timestamp;

	float altitude = packet.getAltitude();
	uint8_t* altitudeBytes = (uint8_t*) &altitude;

	float temperature = packet.getTemperature();
	uint8_t* temperatureBytes = (uint8_t*) &temperature;

	int16_t accelX = packet.getAccelX(); uint8_t* acX = (uint8_t*) &accelX;
	int16_t accelY = packet.getAccelY(); uint8_t* acY = (uint8_t*) &accelY;
	int16_t accelZ = packet.getAccelZ(); uint8_t* acZ = (uint8_t*) &accelZ;
	int16_t gyroX  = packet.getGyroX(); uint8_t* gyrX = (uint8_t*) &gyroX;
	int16_t gyroY  = packet.getGyroY(); uint8_t* gyrY = (uint8_t*) &gyroY;
	int16_t gyroZ  = packet.getGyroZ(); uint8_t* gyrZ = (uint8_t*) &gyroZ;


	Serial.print(PRINT_BEG);			// data string start bytes

    Serial.print(TIMESTAMP);
    Serial.write(timestampBytes[3]);
    Serial.write(timestampBytes[2]);
    Serial.write(timestampBytes[1]);
    Serial.write(timestampBytes[0]);

    Serial.print(STATE);
    Serial.write(0);  					// state zero hardcode for now

    Serial.print(ALTITUDE);
	Serial.write(altitudeBytes[3]);
	Serial.write(altitudeBytes[2]);
	Serial.write(altitudeBytes[1]);
	Serial.write(altitudeBytes[0]);

	Serial.print(TEMPERATURE);
	Serial.write(temperatureBytes[3]);
	Serial.write(temperatureBytes[2]);
	Serial.write(temperatureBytes[1]);
	Serial.write(temperatureBytes[0]);

	// Accelerometer x-axis
	Serial.write(65); // A
	Serial.write(67); // C
	Serial.write(88); // X
	Serial.write(acX[1]);
	Serial.write(acX[0]);

	// Accelerometer y-axis
	Serial.write(65); // A
	Serial.write(67); // C
	Serial.write(89); // Y
	Serial.write(acY[1]);
	Serial.write(acY[0]);

	// Accelerometer z-axis
	Serial.write(65); // A
	Serial.write(67); // C
	Serial.write(90); // Z
	Serial.write(acZ[1]);
	Serial.write(acZ[0]);

	// Gyro x-axis
	Serial.write(71); // G
	Serial.write(89); // Y
	Serial.write(88); // X
	Serial.write(gyrX[1]);
	Serial.write(gyrX[0]);

	// Gyro y-axis
	Serial.write(71); // G
	Serial.write(89); // Y
	Serial.write(89); // Y
	Serial.write(gyrY[1]);
	Serial.write(gyrY[0]);

	// Gyro z-axis
	Serial.write(71); // G
	Serial.write(89); // Y
	Serial.write(90); // Z
	Serial.write(gyrZ[1]);
	Serial.write(gyrZ[0]);


    Serial.print(PRINT_END);			// Data string end bytes


}


/*
 *
 */
//void DataLogger::printPacketToSerialMonitor(DataPacket packet) {
//
//	// Timestamp (and state)
//	uint32_t ts = 0;
//	uint8_t * timestampBytes = (uint8_t *) &ts;
//	timestampBytes[3] = packet.count0;
//	timestampBytes[2] = packet.count1;
//	timestampBytes[1] = packet.count2;
//	timestampBytes[0] = packet.count3;
//	//Serial.print("TIMESTAMP IS: ");
//	Serial.print(ts); Serial.print(", ");
//
//	// Barometer
//	Serial.print(packet.count4); Serial.print(", ");
//	Serial.print(packet.count5); Serial.print(", ");
//	Serial.print(packet.count6); Serial.print(", ");
//	Serial.print(packet.count7); Serial.print(", ");
//
//	// IMU
//	const float ACCEL_LSB = 2048;    // 2048 LSB / G
//	const float GYRO_LSB  = 16.4;	 // 16.4 LSB / (dps)
//
//	int16_t rawAccX, rawAccY, rawAccZ = 0;
//	uint8_t * bytesAccX = (uint8_t *) &rawAccX;
//	uint8_t * bytesAccY = (uint8_t *) &rawAccY;
//	uint8_t * bytesAccZ = (uint8_t *) &rawAccZ;
//
//	int16_t rawGyroX, rawGyroY, rawGyroZ = 0;
//	uint8_t * bytesGyroX = (uint8_t *) &rawGyroX;
//	uint8_t * bytesGyroY = (uint8_t *) &rawGyroY;
//	uint8_t * bytesGyroZ = (uint8_t *) &rawGyroZ;
//
//	bytesAccX[1] = packet.count8;  bytesAccX[0] = packet.count9;
//	bytesAccY[1] = packet.count10; bytesAccY[0] = packet.count11;
//	bytesAccZ[1] = packet.count12; bytesAccZ[0] = packet.count13;
//
//	bytesGyroX[1] = packet.count14; bytesGyroX[0] = packet.count15;
//	bytesGyroY[1] = packet.count16; bytesGyroY[0] = packet.count17;
//	bytesGyroZ[1] = packet.count18; bytesGyroZ[0] = packet.count19;
//
//	float accX = (float) rawAccX * 1 / ACCEL_LSB;
//	float accY = (float) rawAccY * 1 / ACCEL_LSB;
//	float accZ = (float) rawAccZ * 1 / ACCEL_LSB;
//
//	float gyroX = (float) rawGyroX * 1 / GYRO_LSB;
//	float gyroY = (float) rawGyroY * 1 / GYRO_LSB;
//	float gyroZ = (float) rawGyroZ * 1 / GYRO_LSB;
//
//	Serial.print(accX); Serial.print(", ");
//	Serial.print(accY); Serial.print(", ");
//	Serial.print(accZ); Serial.print(", ");
//
//	Serial.print(gyroX); Serial.print(", ");
//	Serial.print(gyroY); Serial.print(", ");
//	Serial.print(gyroZ); //Serial.print(", ");
//
//	Serial.println();
//
//
//
////	float voltage = 0;
////	uint8_t * voltageBytes = (uint8_t *) &voltage;
////	voltageBytes[3] = packet.count4;
////	voltageBytes[2] = packet.count5;
////	voltageBytes[1] = packet.count6;
////	voltageBytes[0] = packet.count7;
//	//Serial.print("VOLTAGE IS: "); Serial.println(voltage);
//	//Serial.println(voltage);
//
////	uint8_t sensorVal = 0;
////	uint8_t * pinBytes = (uint8_t *) &sensorVal;
////	pinBytes[0] = packet.count4;
////	Serial.println(sensorVal);
//
//
//
//}


/*
 *
 */
//void DataLogger::printRawDataToSerialMonitor(DataPacket packet) {
//}


/*
 * Zero subsystem sensors
 */
void DataLogger::zeroSensors() {

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



