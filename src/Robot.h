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


	// Sensors
	ICM20948 * imu = new ICM20948(0x68);
	MPL3115A2 * baro = new MPL3115A2();

	// Data registers of sensors
	//uint8_t baroRegisters[5];
	uint8_t imuRegisters[12];


	LoRaE32 * transceiver = new LoRaE32(&Serial1, PIN_M0, PIN_M1, PIN_AX);
	//LoRaE32 * transceiver = new LoRaE32(&Serial1, E32_LORA_M0, E32_LORA_M1, E32_LORA_AUX);


	struct DATA {
	  uint8_t count0  = 0;
	  uint8_t count1  = 1;
	  uint8_t count2  = 2;
	  uint8_t count3  = 3;
	  uint8_t count4  = 4;
	  uint8_t count5  = 5;
	  uint8_t count6  = 6;
	  uint8_t count7  = 7;
	  uint8_t count8  = 8;
	  uint8_t count9  = 9;
	  uint8_t count10 = 10;
	  uint8_t count11 = 11;
	  uint8_t count12 = 12;
	  uint8_t count13 = 13;
	  uint8_t count14 = 14;
	  uint8_t count15 = 15;
	  uint8_t count16 = 16;
	  uint8_t count17 = 17;
	  uint8_t count18 = 18;
	  uint8_t count19 = 19;
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
