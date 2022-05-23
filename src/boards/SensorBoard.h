/*
 * SensorBoard.h
 *
 *  Created on: May 22, 2022
 *      Author: Daniel Pearson
 */

#ifndef SRC_BOARDS_SENSORBOARD_H_
#define SRC_BOARDS_SENSORBOARD_H_

#include "Arduino.h"
#include "../../Constants.h"

#include "../SystemInterface.h"
#include "../loops/Looper.h"
#include "../loops/loop.h"

class SensorBoard : public SystemInterface {

private:
	BoardStates sensorBoardState = IDLE;


public:
	SensorBoard();

	class SensorBoardLoop : public Loop {
		SensorBoard * sensorBoard_;

		public:
			SensorBoardLoop(SensorBoard * instance){
				sensorBoard_ = instance;
			};

			void onStart(uint32_t timestamp){
				sensorBoard_->beginStateMachine();
			}
			void onLoop(uint32_t timestamp){
				sensorBoard_->updateStateMachine(timestamp);

			}
			void onStop(uint32_t timestamp){
				sensorBoard_->endStateMachine();
			}
	} * sensorBoardLoop = new SensorBoardLoop(this);

	bool systemInit();
	void registerAllLoops(Looper * runningLooper);

	void beginStateMachine();
	void updateStateMachine(uint32_t timestamp);
	void endStateMachine();
};

#endif /* SRC_BOARDS_SENSORBOARD_H_ */
