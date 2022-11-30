/*
 * TelemetryBoard.cpp
 *
 *  Created on: May 22, 2022
 *      Author: Daniel Pearson
 */

#include "TestBoard.h"

TestBoard::TestBoard() {}

bool TestBoard::systemInit() {
	this->setState(SENDING);

	return true;
}

void TestBoard::registerAllLoops(Looper * runningLooper) {
	runningLooper->registerLoop(testBoardLoop);
}

void TestBoard::beginStateMachine() {
}

void TestBoard::setState(BoardStates state) {
	this->testBoardState = state;
	Serial.print("Updated Telemetry Board State: ");
	Serial.println(state);
}

void TestBoard::updateStateMachine(uint32_t timestamp) {
	switch(testBoardState) {
		case IDLE:
			break;

		case INITIALIZE:
			break;

		case SENDING:
			float altitude = 500.45;
			int accel_x = 1000;
			int accel_y = 2000;
			int accel_z = 3000;

			uint8_t * tspB = (uint8_t *) &timestamp;
			uint8_t * altB = (uint8_t *) &altitude;
			uint8_t * acxB = (uint8_t *) &accel_x;
			uint8_t * acyB = (uint8_t *) &accel_y;
			uint8_t * aczB = (uint8_t *) &accel_z;

			Serial.print("BEGB");

			Serial.print("TSP");
			Serial.write(tspB[3]);
			Serial.write(tspB[2]);
			Serial.write(tspB[1]);
			Serial.write(tspB[0]);

			Serial.print("ALT");
			Serial.write(altB[3]);
			Serial.write(altB[2]);
			Serial.write(altB[1]);
			Serial.write(altB[0]);

			Serial.print("ACX");
			Serial.write(acxB[1]);
			Serial.write(acxB[0]);

			Serial.print("ACY");
			Serial.write(acyB[1]);
			Serial.write(acyB[0]);

			Serial.print("ACZ");
			Serial.write(aczB[1]);
			Serial.write(aczB[0]);

			Serial.print("ENDB");
			break;

		default:
			Serial.print("Unknown State Updated: ");
			Serial.println("Test Board");
			break;
	}
}

void TestBoard::endStateMachine() {}
