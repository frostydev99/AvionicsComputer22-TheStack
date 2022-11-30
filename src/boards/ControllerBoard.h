/*
 * ControllerBoard.h
 *
 *  Created on: May 22, 2022
 *      Author: Daniel Pearson
 */

#ifndef SRC_BOARDS_CONTROLLERBOARD_H_
#define SRC_BOARDS_CONTROLLERBOARD_H_

#include "Arduino.h"
#include "../../Constants.h"

#include "../SystemInterface.h"
#include "../loops/Looper.h"
#include "../loops/loop.h"
#include "../peripherals/MCP2515.h"

class ControllerBoard : public SystemInterface {

private:
	BoardStates controllerBoardState = IDLE;
	MCP2515 * canController = new MCP2515(10);
	struct can_frame canMsg;


public:
	ControllerBoard();

	class ControllerBoardLoop : public Loop {
		ControllerBoard * controllerBoard_;

		public:
			ControllerBoardLoop(ControllerBoard * instance){
				controllerBoard_ = instance;
			};

			void onStart(uint32_t timestamp){
				controllerBoard_->beginStateMachine();
			}
			void onLoop(uint32_t timestamp){
				controllerBoard_->updateStateMachine(timestamp);

			}
			void onStop(uint32_t timestamp){
				controllerBoard_->endStateMachine();
			}
	} * controllerBoardLoop = new ControllerBoardLoop(this);

	bool systemInit();
	void registerAllLoops(Looper * runningLooper);

	void beginStateMachine();
	void updateStateMachine(uint32_t timestamp);
	void endStateMachine();
	void setState(BoardStates state);
};

#endif /* SRC_BOARDS_CONTROLLERBOARD_H_ */
