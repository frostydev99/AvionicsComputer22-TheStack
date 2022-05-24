/*
 * PowerBoard.cpp
 *
 *  Created on: May 23, 2022
 *      Author: Christian M. Schrader
 */

#include "PowerBoard.h"

PowerBoard::PowerBoard() {}

bool PowerBoard::systemInit(){

	// Write 0 to servos
	digitalWrite(PB_SERVO_DISABLE, HIGH);
	// Write disabled to MOSFETS

	pinMode(PB_SERVO_DISABLE, OUTPUT);
	pinMode(PB_SERVO_PWM_1, OUTPUT);
	pinMode(PB_SERVO_PWM_2, OUTPUT);
	pinMode(PB_SERVO_PWM_3, OUTPUT);
	pinMode(PB_SERVO_PWM_4, OUTPUT);
	pinMode(PB_VCC, INPUT);
	pinMode(PB_7V, INPUT);

	canController->reset();
	canController->setBitrate(CAN_125KBPS);
	canController->setNormalMode();


	this->setState(SENDING);


	Serial.println("Power Board Initialized");
	return true;
}

void PowerBoard::registerAllLoops(Looper * runningLooper) {
	runningLooper->registerLoop(powerBoardLoop);
}

void PowerBoard::setState(BoardStates state) {
	this->powerBoardState = state;
	Serial.print("Updated Power Board State: ");
	Serial.println(state);
}

void PowerBoard::beginStateMachine() {
	Serial.println("Power Board Started");
}

void PowerBoard::updateStateMachine(uint32_t timestamp) {

	switch(powerBoardState) {
		case SENDING:

			// CAN Receive
			if(canController->readMessage(&canMsg) == MCP2515::ERROR_OK) {
				switch(canMsg.can_id) {
					case CAN_CHARGE_FIRE:

						break;
					case CAN_SERVO_ACTUATE:
						switch(canMsg.data[0])
						{
							case 1:
								servoPin = PB_SERVO_PWM_1;
								break;
							case 2:
								servoPin = PB_SERVO_PWM_2;
								break;
							case 3:
								servoPin = PB_SERVO_PWM_3;
								break;
							case 4:
								servoPin = PB_SERVO_PWM_4;
								break;
						}
						analogWrite(servoPin, map(canMsg.data[1], 0, 360, 0, 255));
						break;
					case CAN_SERVO_POWER:
						digitalWrite(PB_SERVO_DISABLE, canMsg.data[0]);
						break;
				}
			}

			// CAN Send
			if (timestamp - VOLTAGE_READ_INTERVAL > lastVRead)
			{
				lastVRead = timestamp;
				canMsg.can_id  = CAN_VOLTAGE_READ;
				canMsg.can_dlc = DLC_VOLTAGE_READ;

				uint16_t vcc = analogRead(PB_VCC) * VCC_REGRESSION_SLOPE + VCC_REGRESSION_INTERCEPT;
//				uint16_t vcc = analogRead(PB_VCC);
				uint8_t *vccb = (uint8_t *) &vcc;
				canMsg.data[0] = vccb[0];
				canMsg.data[1] = vccb[1];
//			  	canMsg.data[1] = analogRead(PB_7V);
			  	canController->sendMessage(&canMsg);
			}
			break;
		default:
			Serial.print("Unknown State Updated: ");
			Serial.println("Power Board");
			break;

	}
}

void PowerBoard::endStateMachine() {}
