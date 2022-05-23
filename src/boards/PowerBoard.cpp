/*
 * PowerBoard.cpp
 *
 *  Created on: May 23, 2022
 *      Author: Christian M. Schrader
 */

#include "PowerBoard.h"

PowerBoard::PowerBoard() {}

bool PowerBoard::systemInit(){

	// Init CAN
	// Write 0 to servos
	// Write enabled to servo BAT power
	// Write disabled to MOSFETS

//	canMessage.can_id  = CAN_ID;
//	canMessage.can_dlc = CAN_FRAME_LENGTH;
//	canMessage.data[0] = 0x8E;
//	canMessage.data[1] = 0x87;
//	canMessage.data[2] = 0x32;
//	canMessage.data[3] = 0xFA;
//	canMessage.data[4] = 0x26;
//	canMessage.data[5] = 0x8E;
//	canMessage.data[6] = 0xBE;
//	canMessage.data[7] = 0x86;
//
//	canController->reset();
//	canController->setBitrate(CAN_125KBPS);
//	canController->setNormalMode();


	this->setState(SENDING);


	Serial.println("Power Board Initialized");
	return true;
}

void PowerBoard::registerAllLoops(Looper * runningLooper) {
	runningLooper->registerLoop(powerBoardLoop);
}

void PowerBoard::setState(BoardStates state) {
	this->powerBoardState = state;
	Serial.print("Updated Power Board State: ");
	Serial.println(state);
}

void PowerBoard::beginStateMachine() {
	Serial.println("Power Board Started");
}

void PowerBoard::updateStateMachine(uint32_t timestamp) {

	switch(powerBoardState) {
		case SENDING:
//			canController->sendMessage(&canMessage);
//			Serial.println("Message sent from power board");

			// CAN Receive
			if(canController->readMessage(&canMsg) == MCP2515::ERROR_OK) {
				switch(canMsg.can_id) {
					case CAN_CHARGE_FIRE:
						break;
					case CAN_SERVO_ACTUATE:
						break;
					case CAN_SERVO_POWER:
						digitalWrite(PB_SERVO_DISABLE, canMsg.data[0]);
						break;
				}
			}


//			if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
//			    Serial.print(canMsg.can_id, HEX); // print ID
//			    Serial.print(" ");
//			    Serial.print(canMsg.can_dlc, HEX); // print DLC
//			    Serial.print(" ");
//
//			    for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
//			      Serial.print(canMsg.data[i],HEX);
//			      Serial.print(" ");
//			    }


			// RECEIVE CAN COMMAND
			//	SERVO ACTUATION
			// 	SERVO DISABLE
			// 	MOSFET FIRE

			// SEND CAN TELMETRY
			//	BAT VOLTAGE
			//	SERVO VOLTAGE
			break;
		default:
			Serial.print("Unknown State Updated: ");
			Serial.println("Power Board");
			break;

	}
}

void PowerBoard::endStateMachine() {}
