/*
 * TelemetryBoard.h
 *
 *  Created on: June 9, 2022
 *      Author: Daniel Pearson
 */

#ifndef SRC_BOARDS_TESTBOARD_H_
#define SRC_BOARDS_TESTBOARD_H_

#include "Arduino.h"
#include "../../Constants.h"

#include "../SystemInterface.h"
#include "../loops/Looper.h"
#include "../loops/loop.h"

class TestBoard : public SystemInterface {

private:
	BoardStates testBoardState = IDLE;


public:
	TestBoard();

	class TestBoardLoop : public Loop {
		TestBoard * testBoard_;

		public:
			TestBoardLoop(TestBoard * instance){
				testBoard_ = instance;
			};

			void onStart(uint32_t timestamp){
				testBoard_->beginStateMachine();
			}
			void onLoop(uint32_t timestamp){
				testBoard_->updateStateMachine(timestamp);

			}
			void onStop(uint32_t timestamp){
				testBoard_->endStateMachine();
			}
	} * testBoardLoop = new TestBoardLoop(this);

	bool systemInit();
	void registerAllLoops(Looper * runningLooper);

	void beginStateMachine();
	void updateStateMachine(uint32_t timestamp);
	void endStateMachine();
	void setState(BoardStates state);
};

#endif /* SRC_BOARDS_TESTBOARD_H_ */
