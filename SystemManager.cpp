/*
 * SystemManager.cpp
 * Created on: Sep 17, 2021
 * Author: Peter Dentch
 */

#include "SystemManager.h"

/*
* Constructor for this object SystemManager, should only be one instance running
*/
SystemManager::SystemManager(){};

/*
 * Setup function for the system manager
 *
 * When main system is instantiated, subsystems are fields which get defined but initialized in this function
 * Main system has SPI/I2C/Serial objects which are defined and passed to their subsystems
 * Subsystems have init functions, pin bindings/config of peripherals happen there, not in any class constructor
 */
void SystemManager::mainSetup(){

	Wire.begin();									// initialize I2C bus
	Wire.setClock(I2C_FREQ);						// set its frequency

	SPI.begin();									// initialize SPI bus
	// Test with setting different higher freq SPI

#ifdef USE_SERIAL
	// "Serial.begin() is optional on Teensy. USB hardware initialization is performed before setup() runs.
	// The baud rate input is ignored, and only used for Arduino compatibility."
	Serial.begin(BAUD);								// initialize USB port serial, gotta debug somehow
	Serial.println(F("Serial initialized"));
#endif


//#ifdef USE_ROBOT_SYSTEM
//	robot->systemInit();				// initializing system object
//	robot->registerAllLoops(looper);    // and registering its system/subsystem loops
//#endif

	//Individual Boards
	//sensorBoard->systemInit(); //Initializing Sensor Board Object
	//sensorBoard->registerAllLoops(looper); //Registering the sensorBoard loops
	telemetryBoard->systemInit();
	telemetryBoard->registerAllLoops(looper);


}

/*
 * Main loop function of the system manager which is constantly iterated by the Arduino/Teensy loop()
 * This runs the highest level state machine of the computer, handles looper functionality,
 * calling to run the system loops when their setup is successfully and it is safe to do so
 */
void SystemManager::mainLoop(){

	// Main system state machine
	switch(state){

	// Startup state, for starting loops, which should run system/subsystem initialization code that
	// doesn't need to run in setup() unlike hardware peripheral instances
	case Startup:

		// Don't attempt to run loops if they failed to start, stuck in this state
		if(looper->startLoops()){	// start all loops, call all onStart() methods
			state = Running;
			Serial.println("Now in Running Mode");
		}

		break;


	case Running:

		looper->runLoops();			// run all loops, call all onLoop() methods

		break;


	default:
		break;


	}



}


