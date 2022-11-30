/*
 * ControllerBoard.cpp
 *
 *  Created on: May 22, 2022
 *      Author: Daniel Pearson
 */

#include "ControllerBoard.h"

ControllerBoard::ControllerBoard() {}

bool ControllerBoard::systemInit(){
	canMsg.can_id = 501;
	canMsg.can_dlc = 8;
	canMsg.data[0] = 1;
	canMsg.data[1] = 200;

	canController->reset();
	canController->setBitrate(CAN_125KBPS);
	canController->setNormalMode();

	pinMode(3, OUTPUT);

	this->setState(SENDING);


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
			canController->sendMessage(&canMsg);
			analogWrite(3, 1);
			break;

		default:
			Serial.print("Unknown State Updated: ");
			Serial.println("Controller Board");
			break;

	}
}

void ControllerBoard::setState(BoardStates state) {
	this->controllerBoardState = state;
	Serial.print("Updated Telemetry Board State: ");
	Serial.println(state);
}

void ControllerBoard::endStateMachine() {}
