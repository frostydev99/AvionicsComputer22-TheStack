/*
 * SystemManager.h
 * Created on: Sep 17, 2021
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

//Individual Boards
#include "src/boards/SensorBoard.h"
#include "src/boards/ControllerBoard.h"
#include "src/boards/TelemetryBoard.h"
#include "src/boards/PowerBoard.h"


/*
 * Overall system states, main loop execution with everything necessary before and after
 */
enum SystemState {

	Startup,
	//WaitForConnect,
	Running
	//Halting
	//ESTOP
};


class SystemManager{

private:

	SystemState state = Startup;			// initial system state is Startup

	Looper * looper = new Looper();			// looper keeps track of the loops the system will be running


public:

	SystemManager();
	//	~SystemManager(){}


	const uint32_t I2C_FREQ = I2C_BUS_FREQUENCY; 	// frequency of the I2C bus for peripheral interfacing
	const uint32_t SPI_FREQ = SPI_SCK_FREQUENCY;	// frequency of the SPI SCK clock for peripheral interfacing
	const uint32_t BAUD = DEBUG_BAUD_RATE;			// baud rate for serial prints from the Teensy


//#ifdef USE_ROBOT_SYSTEM
	//Robot * robot = new Robot();
//#endif

	//Individual Boards
	//SensorBoard * sensorBoard = new SensorBoard();
	//ControllerBoard * controllerBoard = new ControllerBoard();
	//TelemetryBoard * telemetryBoard = new TelemetryBoard();
	//PowerBoard * powerBoard = new PowerBoard();

	void mainSetup();
	void mainLoop();

};



#endif /* SYSTEMMANAGER_H_ */
