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

class PowerBoard : public SystemInterface {

private:
	BoardStates powerBoardState = IDLE;
	MCP2515 * canController = new MCP2515(8);
	struct can_frame canMsg;
	int servoPin;
	int lastVRead = 0;

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
