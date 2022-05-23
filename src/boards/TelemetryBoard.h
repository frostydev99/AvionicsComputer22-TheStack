/*
 * TelemetryBoard.h
 *
 *  Created on: May 22, 2022
 *      Author: Daniel Pearson
 */

#ifndef SRC_BOARDS_TELEMETRYBOARD_H_
#define SRC_BOARDS_TELEMETRYBOARD_H_

#include "Arduino.h"
#include "../../Constants.h"

#include "../SystemInterface.h"
#include "../loops/Looper.h"
#include "../loops/loop.h"

//Peripherals
#include "../peripherals/mcp2515.h"

#define CAN_FRAME_LENGTH 8
#define CAN_ID 0x0F6

class TelemetryBoard : public SystemInterface {

private:
	BoardStates telemetryBoardState = IDLE;
	MCP2515 * canController = new MCP2515(10);
	struct can_frame canMessage;


public:
	TelemetryBoard();

	class TelemetryBoardLoop : public Loop {
		TelemetryBoard * telemetryBoard_;

		public:
			TelemetryBoardLoop(TelemetryBoard * instance){
				telemetryBoard_ = instance;
			};

			void onStart(uint32_t timestamp){
				telemetryBoard_->beginStateMachine();
			}
			void onLoop(uint32_t timestamp){
				telemetryBoard_->updateStateMachine(timestamp);

			}
			void onStop(uint32_t timestamp){
				telemetryBoard_->endStateMachine();
			}
	} * telemetryBoardLoop = new TelemetryBoardLoop(this);

	bool systemInit();
	void registerAllLoops(Looper * runningLooper);

	void beginStateMachine();
	void updateStateMachine(uint32_t timestamp);
	void endStateMachine();
	void setState(BoardStates state);
};

#endif /* SRC_BOARDS_TELEMETRYBOARD_H_ */
