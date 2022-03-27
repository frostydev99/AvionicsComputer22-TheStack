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


	// Flash memory



	// Transceiver timer
	const uint32_t TRANSMIT_PERIOD = 100;						// ms, telemetry sending period
	MetroTimer transmitTimer = MetroTimer(TRANSMIT_PERIOD);		// passed to timer in milliseconds

	// Transceiver radio
	uint8_t PIN_M0  = E32_LORA_M0;
	uint8_t PIN_M1  = E32_LORA_M1;
	uint8_t PIN_AUX = E32_LORA_AUX;

	LoRaE32 * transceiver = new LoRaE32(&Serial1, PIN_M0, PIN_M1, PIN_AUX);

	// Functions
	bool transcieverInit();
	bool timeToTransmit();



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
