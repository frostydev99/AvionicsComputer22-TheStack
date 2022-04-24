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

#include "../utilities/RocketDataPacket.h"

#include "../peripherals/SerialFlash.h"
#include "../peripherals/LoRaE32.h"


#define PACKET_SIZE 32		// number of bytes in a packet of data to log


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
	DATALOGGER_IDLE,
	//DATALOGGER_WRITE_FILE 			// writing to a flight file
	//DATALOGGER_COPY_BUFFER
	DATALOGGER_RECEIVE_TELEM

} DataLoggerState;



/*
 * The DataLogger subsystem object
 */
class DataLogger : public SubsystemInterface {

private:

	DataLoggerState loggerState = DATALOGGER_STARTUP;		// initial state is startup

	// Memory for current data packet
	uint8_t currentDataPacket[PACKET_SIZE];
	uint32_t dataPacketSize = sizeof(currentDataPacket);	// should be equal to PACKET_SIZE

	// Memory for a buffer of data packets
	uint8_t dataPacketSmallBuffer[30][PACKET_SIZE];
	uint8_t smallBufferIndex = 0;							// current index of packet buffer


	// Flash memory pin
	const uint8_t PIN_CS = SPI_FLASH_CS;

	// Files
	SerialFlashFile bufferFile;


	// Circular buffer file parameters
	const char *bufferFileName = "CircularBuffer.txt";

	uint32_t begDataAddress = 0;
	uint32_t endDataAddress = 0;

	// File size of the buffer is 4 x block size, with block size = 65536 (2^16)
	uint32_t circularBufferSize = 250 * SerialFlash.blockSize();



	// Transceiver timer
	const uint32_t TRANSMIT_PERIOD = 100;						// ms, telemetry sending period
	MetroTimer transmitTimer = MetroTimer(TRANSMIT_PERIOD);		// passed to timer in ms

	uint8_t timer = 0;


	// Transceiver radio pins
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

	void receiveTelemetry();

	//void parseDataForGroundstation();
	void printCurrentPacketToGroundstation();
//	void printPacketToSerialMonitor(DataPacket packet);
//	void printRawDataToSerialMonitor(DataPacket packet);


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
			Serial.print(F("Buffer file size: ")); Serial.println(logger_->circularBufferSize);

			logger_->locateCircBufferAddresses();

			Serial.print("Buffer begin addr: "); Serial.println(logger_->begDataAddress);
			Serial.print("Buffer end addr: "); Serial.println(logger_->endDataAddress);


		}

		void onLoop(uint32_t timestamp){

			// TODO handleRadioTransmit()
//			if(logger_->timeToTransmit()) {
//
//				logger_->transmitTelemetry();
//
//				Serial.print(F("TRANSMIT at: "));
//				Serial.println(timestamp);
//			}


			// Main system state machine
			switch(logger_->loggerState){

			case DATALOGGER_STARTUP:

				//Serial.println("LOGGER STARTUP");

//				logger_->setState(DATALOGGER_WRITE_BUFFER);

//				logger_->setState(DATALOGGER_READ_FILE);
//				logger_->bufferFile.seek( 0 );  // 15372000


				logger_->setState(DATALOGGER_RECEIVE_TELEM);

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

			case DATALOGGER_RECEIVE_TELEM:

				// Check for new received telemetry message to print
				logger_->receiveTelemetry();

				break;

			default:
				break;

			}



		}

		void onStop(uint32_t timestamp){

		}
	} * loggerLoop = new DataLoggerLoop(this);



	void setState(DataLoggerState state);
	void setCurrentDataPacket(const void* packet, uint8_t packetSize);
	void getCurrentDataPacket(void* packet, uint8_t packetSize);

	bool subsystemInit();
	void zeroSensors();
	void registerLoops(Looper * runningLooper);
	void printOutput();

};




#endif /* SRC_SUBSYSTEMS_DATALOGGER_H_ */
