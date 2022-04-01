/*
 * DataLogger.h
 *
 * Created on: Mar 26, 2022
 * Author: Peter Dentch
 */

#ifndef SRC_SUBSYSTEMS_DATALOGGER_H_
#define SRC_SUBSYSTEMS_DATALOGGER_H_

#include "SubsystemInterface.h"
#include "../../Constants.h"

#include "../peripherals/SerialFlash.h"
#include "../peripherals/LoRaE32.h"


// Groundstation identifiers
#define STATE 		"STT"
#define TIMESTAMP 	"TSP"
#define ALTITUDE 	"ALT"
#define TEMPERATURE "TMP"

#define PRINT_BEG	"BEGB"
#define PRINT_END	"ENDB"


/*
 *	States of the DataLogger subsystem state machine
 */
typedef enum {
	DATALOGGER_STARTUP,
	DATALOGGER_WRITE_BUFFER,			// writing to the circular buffer
	DATALOGGER_ERASE_BUFFER,			// erasing a block in the circular buffer
	DATALOGGER_READ_FILE,
	DATALOGGER_IDLE
	//DATALOGGER_WRITE_FILE 			// writing to a flight file
	//DATALOGGER_COPY_BUFFER

} DataLoggerState;


/*
 * Data packet structure for logging, transmitting, and printing to the groundstation
 * Size is 32 bytes
 */
struct DataPacket {
	uint8_t count0  = 0;  uint8_t count1  = 1;  uint8_t count2  = 2;  uint8_t count3  = 3;
	uint8_t count4  = 4;  uint8_t count5  = 5;  uint8_t count6  = 6;  uint8_t count7  = 7;
	uint8_t count8  = 8;  uint8_t count9  = 9;  uint8_t count10 = 10; uint8_t count11 = 11;
	uint8_t count12 = 12; uint8_t count13 = 13; uint8_t count14 = 14; uint8_t count15 = 15;
	uint8_t count16 = 16; uint8_t count17 = 17; uint8_t count18 = 18; uint8_t count19 = 19;
	uint8_t count20 = 20; uint8_t count21 = 21; uint8_t count22 = 22; uint8_t count23 = 23;
	uint8_t count24 = 24; uint8_t count25 = 25; uint8_t count26 = 26; uint8_t count27 = 27;
	uint8_t count28 = 28; uint8_t count29 = 29; uint8_t count30 = 30; uint8_t count31 = 31;
};


/*
 * The DataLogger subsystem object
 */
class DataLogger : public SubsystemInterface {

private:

	DataLoggerState loggerState = DATALOGGER_STARTUP;		// initial state is startup

	// Data
	DataPacket currentDataPacket;
	uint32_t dataPacketSize = sizeof(currentDataPacket);

	DataPacket dataPacketSmallBuffer[30];
	uint8_t smallBufferIndex = 0;							// current index of packet buffer


	// Flash memory
	const uint8_t PIN_CS = SPI_FLASH_CS;

	// Files
	SerialFlashFile bufferFile;


	// Circular buffer file parameters
	const char *bufferFileName = "CircularBuffer.txt";

	uint32_t begDataAddress = 0;
	uint32_t endDataAddress = 0;

	// File size of the buffer is 4 x block size, with block size = 65536 (2^16)
	uint32_t circularBufferSize = 4 * SerialFlash.blockSize(); 		// = 262144



	// Transceiver timer
	const uint32_t TRANSMIT_PERIOD = 100;						// ms, telemetry sending period
	MetroTimer transmitTimer = MetroTimer(TRANSMIT_PERIOD);		// passed to timer in ms

	// Transceiver radio
	const uint8_t PIN_M0  = E32_LORA_M0;
	const uint8_t PIN_M1  = E32_LORA_M1;
	const uint8_t PIN_AUX = E32_LORA_AUX;

	LoRaE32 * transceiver = new LoRaE32(&Serial1, PIN_M0, PIN_M1, PIN_AUX);


	// Functions
	bool flashMemoryInit();
	void locateCircBufferAddresses();
	bool updateCircBuffer();
	bool readCircBuffer();
	bool addPacketToSmallBuffer();
	void writeSmallBufferToCircBuffer();

	bool transcieverInit();
	bool timeToTransmit();
	bool transmitTelemetry();

	//void parseDataForGroundstation();
	void printPacketToGroundstation(DataPacket packet);
	void printPacketToSerialMonitor(DataPacket packet);


public:

	DataLogger();
	//	~DataLogger(){}

	/* DataLogger loop functionality */
	class DataLoggerLoop : public Loop {
		DataLogger * logger_;

	public:
		DataLoggerLoop(DataLogger * instance){
			logger_ = instance;
		}

		void onStart(uint32_t timestamp){

			Serial.print(F("Logger state: ")); Serial.println(logger_->loggerState);

			logger_->locateCircBufferAddresses();

			Serial.print("Buffer begin addr: "); Serial.println(logger_->begDataAddress);
			Serial.print("Buffer end addr: "); Serial.println(logger_->endDataAddress);


		}

		void onLoop(uint32_t timestamp){

			// Main system state machine
			switch(logger_->loggerState){

			case DATALOGGER_STARTUP:

				Serial.println("LOGGER STARTUP");

				logger_->setState(DATALOGGER_WRITE_BUFFER);

//				logger_->setState(DATALOGGER_READ_FILE);
//				logger_->bufferFile.seek(0);

				break;

			case DATALOGGER_WRITE_BUFFER:

				// Let block erase command finish if began
				if(!logger_->updateCircBuffer()){

					logger_->addPacketToSmallBuffer();
					logger_->setState(DATALOGGER_ERASE_BUFFER);
				}

				break;

			case DATALOGGER_ERASE_BUFFER:

				//Serial.println(F("CHIP BUSY"));

				logger_->addPacketToSmallBuffer();

				// Continue writing once erase command is finished
				if(SerialFlash.ready()){

					logger_->writeSmallBufferToCircBuffer();
					logger_->setState(DATALOGGER_WRITE_BUFFER);
				}

				break;

			case DATALOGGER_READ_FILE:

				// Idle if finished reading
				if(!logger_->readCircBuffer()){
					logger_->setState(DATALOGGER_IDLE);
				}

				break;

			case DATALOGGER_IDLE:
					// no-op
				break;

			default:
				break;

			}


			//if(logger_->timeToTransmit()) {
			//	Serial.println(F("TRANSMIT"));
			//}

		}

		void onStop(uint32_t timestamp){

		}
	} * loggerLoop = new DataLoggerLoop(this);



	void setState(DataLoggerState state);
	void setCurrentDataPacket(DataPacket packet);


	bool subsystemInit();
	void zeroSensors();
	void registerLoops(Looper * runningLooper);
	void printOutput();

};




#endif /* SRC_SUBSYSTEMS_DATALOGGER_H_ */
