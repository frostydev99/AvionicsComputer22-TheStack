/*
 * ControllerBoard.cpp
 *
 *  Created on: May 22, 2022
 *      Author: Daniel Pearson
 */

#include "ControllerBoard.h"

ControllerBoard::ControllerBoard() {}

bool ControllerBoard::systemInit(){


	Serial.println("Controller Board Initialized");
	return true;
}

void ControllerBoard::registerAllLoops(Looper * runningLooper) {
	runningLooper->registerLoop(controllerBoardLoop);
}

void ControllerBoard::beginStateMachine() {
	Serial.println("Controller Board Started");
}

void ControllerBoard::updateStateMachine(uint32_t timestamp) {

	switch(controllerBoardState) {
		case IDLE:
			break;

		case INITIALIZE:
			break;

		case SENDING:
			break;

		default:
			Serial.print("Unknown State Updated: ");
			Serial.println("Controller Board");
			break;

	}
}

void ControllerBoard::endStateMachine() {}
