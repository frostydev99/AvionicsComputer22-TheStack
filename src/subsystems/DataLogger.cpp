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



	// Transceiver radio
	bool radioInit = false;
	radioInit = transcieverInit();


	return radioInit;

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
bool DataLogger::timeToTransmit() {
	return transmitTimer.check() == 1;			//check if the timer has passed it's interval
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



