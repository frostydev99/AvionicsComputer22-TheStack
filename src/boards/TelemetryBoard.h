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

class TelemetryBoard : public SystemInterface {

private:
	BoardStates telemetryBoardState = IDLE;


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
};

#endif /* SRC_BOARDS_TELEMETRYBOARD_H_ */
