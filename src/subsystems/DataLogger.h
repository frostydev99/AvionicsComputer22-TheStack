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


/*
 *	States of the DataLogger subsystem state machine
 */
typedef enum {
	DATALOGGER_STARTUP,
	DATALOGGER_LOGGING,
	DATALOGGER_READ_FILE,
	DATALOGGER_IDLE
} DataLoggerState;


/*
 * The DataLogger subsystem object
 */
class DataLogger : public SubsystemInterface {

private:

	DataLoggerState loggerState = DATALOGGER_STARTUP;		// initial state is startup


	struct DATA {
	  uint8_t count0  = 0;  uint8_t count1  = 1;  uint8_t count2  = 2;  uint8_t count3  = 3;
	  uint8_t count4  = 4;  uint8_t count5  = 5;  uint8_t count6  = 6;  uint8_t count7  = 7;
	  uint8_t count8  = 8;  uint8_t count9  = 9;  uint8_t count10 = 10; uint8_t count11 = 11;
	  uint8_t count12 = 12; uint8_t count13 = 13; uint8_t count14 = 14; uint8_t count15 = 15;
	  uint8_t count16 = 16; uint8_t count17 = 17; uint8_t count18 = 18; uint8_t count19 = 19;
	  uint8_t count20 = 20; uint8_t count21 = 21; uint8_t count22 = 22; uint8_t count23 = 23;
	  uint8_t count24 = 24; uint8_t count25 = 25; uint8_t count26 = 26; uint8_t count27 = 27;
	  uint8_t count28 = 28; uint8_t count29 = 29; uint8_t count30 = 30; uint8_t count31 = 31;
	};
	DATA dataPacket;

	uint32_t dataPacketSize = sizeof(dataPacket);


	// Flash memory
	const uint8_t PIN_CS = SPI_FLASH_CS;

	const char *fileName = "CircularBuffer.txt";

	// File size of the buffer is 4 x block size, with block size = 65536 (2^16)
	uint32_t circularBufferSize = 4 * SerialFlash.blockSize();

	SerialFlashFile file;



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

	bool transcieverInit();
	bool timeToTransmit();
	bool transmitTelemetry();



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

		}

		void onLoop(uint32_t timestamp){

			if(logger_->timeToTransmit()) {
				Serial.println(F("TRANSMIT"));

			}
		}

		void onStop(uint32_t timestamp){

		}
	} * loggerLoop = new DataLoggerLoop(this);


	bool subsystemInit();
	void zeroSensors();
	void registerLoops(Looper * runningLooper);
	void printOutput();

};




#endif /* SRC_SUBSYSTEMS_DATALOGGER_H_ */
