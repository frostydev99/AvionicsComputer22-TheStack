/*
 * PowerBoard.cpp
 *
 *  Created on: May 23, 2022
 *      Author: Christian M. Schrader
 */

#include "PowerBoard.h"

PowerBoard::PowerBoard() {}

bool PowerBoard::systemInit(){


	Serial.println("Power Board Initialized");
	return true;
}

void PowerBoard::registerAllLoops(Looper * runningLooper) {
	runningLooper->registerLoop(PowerBoardLoop);
}

void PowerBoard::beginStateMachine() {
	Serial.println("Power Board Started");
}

void PowerBoard::updateStateMachine(uint32_t timestamp) {

	switch(powerBoardState) {
	case INITIALIZE:
				// Init CAN
				// Write 0 to servos
				// Write enabled to servo BAT power
				// Write disabled to MOSFETS

				break;

			case SENDING:
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
