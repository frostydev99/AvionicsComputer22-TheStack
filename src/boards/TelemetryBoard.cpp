/*
 * TelemetryBoard.cpp
 *
 *  Created on: May 22, 2022
 *      Author: Daniel Pearson
 */

#include "TelemetryBoard.h"

TelemetryBoard::TelemetryBoard() {}

bool TelemetryBoard::systemInit(){


	Serial.println("Sensor Board Initialized");
	return true;
}

void TelemetryBoard::registerAllLoops(Looper * runningLooper) {
	runningLooper->registerLoop(telemetryBoardLoop);
}

void TelemetryBoard::beginStateMachine() {
	Serial.println("Sensor Board Started");
}

void TelemetryBoard::updateStateMachine(uint32_t timestamp) {

	switch(telemetryBoardState) {
		case IDLE:
			break;

		case INITIALIZE:
			break;

		case SENDING:
			break;

		default:
			Serial.print("Unknown State Updated: ");
			Serial.println("Telemetry Board");
			break;

	}
}

void TelemetryBoard::endStateMachine() {}
