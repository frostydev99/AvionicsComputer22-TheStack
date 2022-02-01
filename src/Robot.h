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

//#include "subsystems/DriveTrain.h"

//#include "peripheral/GyroAccel.h"
//#include "peripheral/LoRaRadio.h"

#include "peripherals/LoRaE32.h"


// TODO make these properly
#define PIN_M0 2
#define PIN_M1 3
#define PIN_AX 4


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

//	MPU9250 * robotIMU = new MPU6050();
//	DriveTrain * driveTrain = new DriveTrain(robotIMU);


	LoRaE32 * transceiver = new LoRaE32(&Serial1, PIN_M0, PIN_M1, PIN_AX);

	struct DATA {
	  uint8_t Count;
	};
	DATA MyData;



public:

	Robot();
	//	~Robot(){}

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
