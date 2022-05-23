/*
 * TelemetryBoard.cpp
 *
 *  Created on: May 22, 2022
 *      Author: Daniel Pearson
 */

#include "TelemetryBoard.h"

TelemetryBoard::TelemetryBoard() {}

bool TelemetryBoard::systemInit(){

	canController->reset();
	canController->setBitrate(CAN_125KBPS);
	canController->setNormalMode();

	canMessage.can_id = CAN_ID;
	canMessage.can_dlc = CAN_FRAME_LENGTH;
	canMessage.data[0] = 0x48;
	canMessage.data[1] = 0x45;
	canMessage.data[2] = 0x4C;
	canMessage.data[3] = 0x4C;
	canMessage.data[4] = 0x4F;
	canMessage.data[5] = 0x21;
	canMessage.data[6] = 0x21;
	canMessage.data[7] = 0x21;


	this->setState(SENDING);


	Serial.println("Sensor Board Initialized");
	return true;
}

void TelemetryBoard::registerAllLoops(Looper * runningLooper) {
	runningLooper->registerLoop(telemetryBoardLoop);
}

void TelemetryBoard::beginStateMachine() {
	Serial.println("Sensor Board Started");
}

void TelemetryBoard::setState(BoardStates state) {
	this->telemetryBoardState = state;
	Serial.print("Updated Telemetry Board State: ");
	Serial.println(state);
}

void TelemetryBoard::updateStateMachine(uint32_t timestamp) {
	switch(telemetryBoardState) {
		case IDLE:
			break;

		case INITIALIZE:
			break;

		case SENDING:
			canController->sendMessage(&canMessage);
			Serial.println("Messages Sent from telemetry board");
			break;

		default:
			Serial.print("Unknown State Updated: ");
			Serial.println("Telemetry Board");
			break;
	}
}

void TelemetryBoard::endStateMachine() {}
