/*
 * TelemetryBoard.cpp
 *
 *  Created on: May 22, 2022
 *      Author: Daniel Pearson
 */

#include "TelemetryBoard.h"

TelemetryBoard::TelemetryBoard() {}

bool TelemetryBoard::systemInit(){

	//CAN Initialization
	canController->reset();
	canController->setBitrate(CAN_125KBPS);
	canController->setNormalMode();

	dataLogger->subsystemInit(); //Initialize data logger, for sending and receiving of LoRa messages

	this->setState(SENDING); // Change board to sending state, ready to RX/TX CAN Messages


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
			//Variables must be initialized here, for unknown reasons the exact same code in the header file refuses to loop
			uint32_t * tempAndPressure;

			int16_t * accelX;
			int16_t * accelY;
			int16_t * accelZ;

			int16_t * gyroX;
			int16_t * gyroY;
			int16_t * gyroZ;

			if (canController->readMessage(&canMessage) == MCP2515::ERROR_OK) {
			    Serial.print(canMessage.can_id); // print ID
			    Serial.print(" ");
			    Serial.print(canMessage.can_dlc); // print DLC
			    Serial.print(" ");

			    for (int i = 0; i < canMessage.can_dlc; i++)  {  // print the data
			      Serial.print(canMessage.data[i]);
			      Serial.print(" ");
			    }

			    Serial.println();
			 }

			//If a message is received
			if(&canMessage) {
				tempAndPressure[0] = canMessage.data[0]; //uint32_t
				tempAndPressure[1] = canMessage.data[1];
				tempAndPressure[2] = canMessage.data[2];
				tempAndPressure[3] = canMessage.data[3];

				accelX[0] = canMessage.data[4]; //int16_t
				accelX[1] = canMessage.data[5];

				accelY[0] = canMessage.data[6]; //int16_t
				accelY[1] = canMessage.data[7];

				accelZ[0] = canMessage.data[8]; //int16_t
				accelZ[1] = canMessage.data[9];

				gyroX[0] = canMessage.data[10]; //int16_t
				gyroY[1] = canMessage.data[11];

				gyroZ[0] = canMessage.data[12]; //int16_t
				gyroZ[1] = canMessage.data[13];


				//packet.setTimestamp(timestamp);
				packet.setState(0); //Hard coded to zero for now!
				packet.setAltAndTempCombined(*tempAndPressure);

				//packet.setAccelX(*accelX);
				packet.setAccelY(*accelY);
				packet.setAccelZ(*accelZ);

				packet.setGyroX(*gyroX);
				packet.setGyroY(*gyroY);
				packet.setGyroZ(*gyroZ);

				packet.updateToTelemPacket();

				dataLogger->setCurrentDataPacket(packet.getTelemRocketPacketPtr(), 20);

			}

			break;

		default:
			Serial.print("Unknown State Updated: ");
			Serial.println("Telemetry Board");
			break;
	}
}

void TelemetryBoard::endStateMachine() {}
