/*
 * SystemManager.h
 * Created on: Sept 12, 2021
 * Author: Peter Dentch
 */

#ifndef SYSTEMMANAGER_H_
#define SYSTEMMANAGER_H_

#include "Arduino.h"

#include <Wire.h>
#include <SPI.h>

#include "Constants.h"
#include "src/loops/Looper.h"
#include "src/loops/Loop.h"
#include "src/Robot.h"

/*
 * Overall system states, main loop execution with everything necessary before and after
 */
enum SystemState {

	// Highest level system states, main loop execution with everything necessary before and after
	Startup,
	WaitForConnect,
	Running
	//, Halting
	//, ESTOP
};


/*
 * Class SystemManager is the starting point of all TeensyRobotComputer code, it manages
 * the iterative functionality of the system/subsystems. Subsystems are defined by the system
 * on which this code is running, for this program only the soil-sampling robotic drive base
 */
class SystemManager{

private:

	SystemState state;			// initial system state is Startup

	Looper * looper;			// looper keeps track of the loops the system will be running


public:

	SystemManager();
	//	~SystemManager(){}


	const uint32_t i2c_freq = I2C_BUS_FREQUENCY; 	// frequency of the I2C bus for peripheral interfacing
	const uint32_t spi_freq = SPI_SCK_FREQUENCY;	// frequency of the SPI SCK clock for peripheral interfacing
	const uint32_t baud 	= SERIAL_BAUD_RATE;		// baud rate for serial prints from the Teensy


	Robot * robot = new Robot(looper);	// looper is needed for registering all subsystem loops


//	long runCount = 100000;	// debugging, stopping loops after this many mainLoop() iterations


	void mainSetup();

	void mainLoop();

};


#endif /* SYSTEMMANAGER_H_ */
