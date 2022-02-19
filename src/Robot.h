/*
 * Robot.h
 * Created on: Sep 12, 2021
 * Author: Peter Dentch
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

#include "Arduino.h"

#include "../Constants.h"
#include "SystemInterface.h"
#include "loops/Looper.h"
#include "loops/loop.h"

#include "peripherals/ICM20948.h"
#include "peripherals/MPL3115A2.h"

/*
 * Robot has TODO primary states of autonomous operation throughout its mission which begins when the system is powered on
 *
 * STATE DETAILS
 */
enum RobotState {

	//TESTING,
	//Waiting,
	//Driving
	IDLE
};

/*
 * The Robot object, instantiates all robot subsystems and registers their loops
 *
 * DETAILS
 *
 */
class Robot : public SystemInterface {

private:

	RobotState robotState = IDLE;			// initial system state is IDLE

	// Sensors
	ICM20948 * imu = new ICM20948(0x68);
	MPL3115A2 * baro = new MPL3115A2();

	// Data registers of sensors
	uint8_t baroRegisters[6];
	uint8_t imuRegisters[14];



public:

	Robot();
	//	Robot(){}

	/* Robot loop functionality */
	class RobotLoop : public Loop {
		Robot * robot_;

	public:
		RobotLoop(Robot * instance){
			robot_ = instance;
		};

		void onStart(uint32_t timestamp){
			robot_->beginStateMachine();
		}
		void onLoop(uint32_t timestamp){
			robot_->updateStateMachine(timestamp);
		}
		void onStop(uint32_t timestamp){
			robot_->endStateMachine();
		}
	} * robotLoop = new RobotLoop(this);		// instantiate the main system loop and pass it the system instance

	bool systemInit();
	void registerAllLoops(Looper * runningLooper);

	void zeroAllSensors();

	void beginStateMachine();
	void updateStateMachine(uint32_t timestamp);
	void endStateMachine();

};


#endif /* SRC_ROBOT_H_ */
