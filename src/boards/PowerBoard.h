/*
 * PowerBoard.h
 *
 *  Created on: May 23, 2022
 *      Author: Christian M. Schrader
 */

#ifndef SRC_BOARDS_POWERBOARD_H_
#define SRC_BOARDS_POWERBOARD_H_

#include "Arduino.h"
#include "../../Constants.h"

#include "../SystemInterface.h"
#include "../loops/Looper.h"
#include "../loops/loop.h"

//Peripherals
#include "../peripherals/mcp2515.h"
#include "../peripherals/Servo.h"

class PowerBoard : public SystemInterface {

private:
	BoardStates powerBoardState = IDLE;
	MCP2515 * canController = new MCP2515(PB_MCP515);
	Servo * servo1 = new Servo(PB_SERVO_PWM_1, SERVO1_RANGE_LOWER, SERVO1_RANGE_UPPER, SERVO1_LIMIT_LOWER, SERVO1_LIMIT_UPPER);
	Servo * servo2 = new Servo(PB_SERVO_PWM_2, SERVO2_RANGE_LOWER, SERVO2_RANGE_UPPER, SERVO2_LIMIT_LOWER, SERVO2_LIMIT_UPPER);
	Servo * servo3 = new Servo(PB_SERVO_PWM_3, SERVO3_RANGE_LOWER, SERVO3_RANGE_UPPER, SERVO3_LIMIT_LOWER, SERVO3_LIMIT_UPPER);
	Servo * servo4 = new Servo(PB_SERVO_PWM_4, SERVO4_RANGE_LOWER, SERVO4_RANGE_UPPER, SERVO4_LIMIT_LOWER, SERVO4_LIMIT_UPPER);
	struct can_frame canMsg;
	struct can_frame rxMsg;
	int servoPin;
	Servo* servoSelect;
	uint8_t *anglearr;
	uint32_t lastVRead = 0;
	uint16_t servoAngle;

public:
	PowerBoard();

	class PowerBoardLoop : public Loop {
		PowerBoard * PowerBoard_;

		public:
			PowerBoardLoop(PowerBoard * instance){
				PowerBoard_ = instance;
			};

			void onStart(uint32_t timestamp){
				PowerBoard_->beginStateMachine();
			}
			void onLoop(uint32_t timestamp){
				PowerBoard_->updateStateMachine(timestamp);

			}
			void onStop(uint32_t timestamp){
				PowerBoard_->endStateMachine();
			}
	} * powerBoardLoop = new PowerBoardLoop(this);

	bool systemInit();
	void registerAllLoops(Looper * runningLooper);

	void beginStateMachine();
	void updateStateMachine(uint32_t timestamp);
	void endStateMachine();
	void setState(BoardStates state);
};

#endif /* SRC_BOARDS_PowerBoard_H_ */
