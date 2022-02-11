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
 * Should take SPI/I2C/Serial objects as imput parameters?
 */
bool Robot::systemInit(){

	//pinMode(LED_BUILTIN, OUTPUT);		// ! will conflict with CLK if using SPI !

	Serial1.begin(9600);
	transceiver->init();



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

	Serial.println(F("STARTED ROBOT LOOP"));
	//zeroAllSensors();

	//transceiver->SetMode();
	transceiver->SetAddressH(0);
	transceiver->SetAddressL(0);
	transceiver->SetChannel(1);
	//transceiver->SetOptions();

	transceiver->SetParityBit(0);	 		// SpeedParityBit
	transceiver->SetUARTBaudRate(3);		// 3 = 9600 baud
	transceiver->SetAirDataRate(5);			// 2 = B010 =  2.4kbps (default)
											// 5 = B101 = 19.2kbps
	transceiver->SetTransmissionMode(0);	// OptionTrans
	transceiver->SetPullupMode(1);			// OptionPullup
	transceiver->SetWORTIming(0);			// OptionWakeup
	transceiver->SetFECMode(1);				// OptionFEC
	transceiver->SetTransmitPower(0);		// default

	transceiver->SaveParameters(PERMANENT);
	transceiver->PrintParameters();


}


void Robot::updateStateMachine(uint32_t timestamp){


	//digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	//Serial.println(millis());
	//Serial.println(timestamp);

	//float convert = 0.00293255131;
	//Serial.println(0.00293255131 * analogRead(A0) );


	// SENDING
//	if(MyData.Count == 255){
//		MyData.Count = 0;
//	} else {
//		MyData.Count++;
//	}
//
//	transceiver->SendStruct(&MyData, sizeof(MyData));
//
//	//Serial.write(13);				// CR
//	//Serial.write(MyData.Count);
//	//Serial.write(10);				// LF
//
//	Serial.print(F("Sent: "));
//	Serial.println(MyData.Count);


	// RECEIVING
	if(transceiver->available()) {

		transceiver->GetStruct(&MyData, sizeof(MyData));

		//Serial.print(timestamp); Serial.print(F(": "));
		//Serial.println(MyData.Count);

		Serial.print(MyData.Count); Serial.print(F(", "));
		Serial.print(MyData.count1); Serial.print(F(", "));
		Serial.print(MyData.count2); Serial.print(F(", "));
		Serial.print(MyData.count3); Serial.print(F(", "));
		Serial.print(MyData.count4); Serial.print(F(", "));
		Serial.print(MyData.count5); Serial.print(F(", "));
		Serial.print(MyData.count6); Serial.print(F(", "));
		Serial.print(MyData.count7); Serial.print(F(", "));
		Serial.print(MyData.count8); Serial.print(F(", "));
		Serial.print(MyData.count9); Serial.print(F(", "));
		Serial.print(MyData.count10); Serial.print(F(", "));
		Serial.print(MyData.count11); Serial.print(F(", "));
		Serial.print(MyData.count12); Serial.print(F(", "));
		Serial.print(MyData.count13); Serial.print(F(", "));
		Serial.print(MyData.count14); Serial.print(F(", "));
		Serial.print(MyData.count15); Serial.print(F(", "));
		Serial.print(MyData.count16); Serial.print(F(", "));
		Serial.print(MyData.count17);
		Serial.println();
	}


}


void Robot::endStateMachine(){

}
