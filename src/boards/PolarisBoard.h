/*
 * PolarisBoard.h
 *
 *  Created on: Jun 19, 2022
 *      Author: Abby Hyde
 */

#ifndef SRC_BOARDS_POLARISBOARD_H_
#define SRC_BOARDS_POLARISBOARD_H_

#include "Arduino.h"
#include "../../Constants.h"

#include "../SystemInterface.h"
#include "../loops/Looper.h"
#include "../loops/loop.h"

//Peripherals
#include "../peripherals/mcp2515.h"
#include "../peripherals/ICM20948.h"
#include "../peripherals/MPL3115A2.h"
#include "../utilities/RocketDataPacket.h"
#include "../subsystems/DataLogger.h"

#define CAN_FRAME_LENGTH 8
#define CAN_ID 0x00

class PolarisBoard : public SystemInterface {

private:
	BoardStates polarisBoardState = IDLE;
	MCP2515 * canController = new MCP2515(55);
	struct can_frame canMessage;

	ICM20948 * imu = new ICM20948(0);	// NEEDS ADDRESS
	MPL3115A2 * baro = new MPL3115A2();

//	RocketDataPacket packet;
//	DataLogger * dataLogger = new DataLogger();

public:
	PolarisBoard();

	class PolarisBoardLoop : public Loop {
		PolarisBoard * polarisBoard_;

		public:
			PolarisBoardLoop(PolarisBoard * instance){
				polarisBoard_ = instance;
			};

			void onStart(uint32_t timestamp){
				polarisBoard_->beginStateMachine();
			}
			void onLoop(uint32_t timestamp){
				polarisBoard_->updateStateMachine(timestamp);

			}
			void onStop(uint32_t timestamp){
				polarisBoard_->endStateMachine();
			}
	} * polarisBoardLoop = new PolarisBoardLoop(this);

	bool systemInit();
	void registerAllLoops(Looper * runningLooper);

	void beginStateMachine();
	void updateStateMachine(uint32_t timestamp);
	void endStateMachine();
	void setState(BoardStates state);

};

#endif /* SRC_BOARDS_POLARISBOARD_H_ */
