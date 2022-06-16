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

	this->setState(SENDING);


	Serial.println("Telemetry Board Initialized");
	return true;
}

void TelemetryBoard::registerAllLoops(Looper * runningLooper) {
	runningLooper->registerLoop(telemetryBoardLoop);
}

void TelemetryBoard::beginStateMachine() {
	Serial.println("Telemetry Board Started");
}

void TelemetryBoard::setState(BoardStates state) {
	this->telemetryBoardState = state;
	Serial.print("Updated Telemetry Board State: ");
	Serial.println(state);
}

void TelemetryBoard::updateStateMachine(uint32_t timestamp) {
	switch(telemetryBoardState) {
		case IDLE:
			Serial.print("Status: ");
			Serial.println(canController->getStatus());
			break;

		case INITIALIZE:
			break;

		case SENDING:
			if (canController->readMessage(&canMessage) == MCP2515::ERROR_OK) {
			    Serial.print(canMessage.can_id); // print ID
			    Serial.print(" ");
			    Serial.print(canMessage.can_dlc); // print DLC
			    Serial.print(" ");

			    for (int i = 0; i<canMessage.can_dlc; i++)  {  // print the data
			      Serial.print(canMessage.data[i]);
			      Serial.print(" ");
			    }

			    Serial.println();
			 }
			break;

		default:
			Serial.print("Unknown State Updated: ");
			Serial.println("Telemetry Board");
			break;
	}
}

void TelemetryBoard::endStateMachine() {}
