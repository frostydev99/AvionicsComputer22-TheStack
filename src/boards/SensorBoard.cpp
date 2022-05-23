/*
 * SensorBoard.cpp
 *
 *  Created on: May 22, 2022
 *      Author: Daniel Pearson
 */

#include "SensorBoard.h"

SensorBoard::SensorBoard() {}

bool SensorBoard::systemInit(){


	Serial.println("Sensor Board Initialized");
	return true;
}

void SensorBoard::registerAllLoops(Looper * runningLooper) {
	runningLooper->registerLoop(sensorBoardLoop);
}

void SensorBoard::beginStateMachine() {
	Serial.println("Sensor Board Started");
}

void SensorBoard::updateStateMachine(uint32_t timestamp) {

	switch(sensorBoardState) {
		case IDLE:
			break;

		case INITIALIZE:
			break;

		case SENDING:
			break;

		default:
			Serial.print("Unknown State Updated: ");
			Serial.println("Sensor Board");
			break;

	}
}

void SensorBoard::endStateMachine() {}
