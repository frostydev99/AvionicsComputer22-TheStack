/*
 * PolarisBoard.cpp
 *
 *  Created on: Jun 19, 2022
 *      Author: Abby Hyde
 */

#include "PolarisBoard.h"

PolarisBoard::PolarisBoard() {}

bool PolarisBoard::systemInit(){

	//CAN Initialization
	canController->reset();
	canController->setBitrate(CAN_125KBPS);
	canController->setNormalMode();

//	dataLogger->subsystemInit(); //Initialize data logger, for sending and receiving of LoRa messages

	this->setState(SENDING); // Change board to sending state, ready to RX/TX CAN Messages

	Serial.println("Polaris Board Initialized");
	return true;
}

void PolarisBoard::registerAllLoops(Looper * runningLooper) {
	runningLooper->registerLoop(polarisBoardLoop);
}

void PolarisBoard::beginStateMachine() {
	Serial.println("Polaris Board Started");
}

void PolarisBoard::setState(BoardStates state) {
	this->polarisBoardState = state;
	Serial.print("Updated Polaris Board State: ");
	Serial.println(state);
}

void PolarisBoard::updateStateMachine(uint32_t timestamp) {
	switch(polarisBoardState) {
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

			// read data from sensors

			// build CAN message

			if(&canMessage) {
				canMessage.data[0] = tempAndPressure[0];
				canMessage.data[1] = tempAndPressure[1];
				canMessage.data[2] = tempAndPressure[2];
				canMessage.data[3] = tempAndPressure[3];

				canMessage.data[4] = accelX[0];
				canMessage.data[5] = accelY[1];

				canMessage.data[6] = accelX[0];
				canMessage.data[7] = accelY[1];

				canMessage.data[8] = accelX[0];
				canMessage.data[9] = accelY[1];

				canMessage.data[10] = gyroX[0];
				canMessage.data[11] = gyroX[1];

				canMessage.data[12] = gyroY[0];
				canMessage.data[13] = gyroY[1];

				canMessage.data[14] = gyroZ[0];
				canMessage.data[15] = gyroZ[1];

			}

			// send data
			if (canController->sendMessage(&canMessage) == MCP2515::ERROR_OK) {
			    Serial.print(canMessage.can_id); // print ID
			    Serial.print(" ");
			    Serial.print(canMessage.can_dlc); // print DLC
//			    Serial.print(" ");

//			    for (int i = 0; i < canMessage.can_dlc; i++)  {  // print the data
//			      Serial.print(canMessage.data[i]);
//			      Serial.print(" ");
//			    }

			    Serial.println();
			}

			break;

		default:
			Serial.print("Unknown State Updated: ");
			Serial.println("Polaris Board");
			break;
	}
}

