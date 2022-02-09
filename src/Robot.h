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
	MPL3115A2 * baro_ = new MPL3115A2();


//	MPU9250 * robotIMU = new MPU6050();

//	DriveTrain * driveTrain = new DriveTrain(robotIMU);



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
			robot_->baro_->update();
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
