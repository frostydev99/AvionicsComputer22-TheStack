/*
 * Constants.h
 *
 *  Created on: Sep 12, 2021
 *  Author: Peter Dentch
 *
  * Storing all pin wiring and physical constants of the robot computer board
 *
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_




/* ROBOT CONSTANTS */

#define PAYLOAD_ROBOT_NUM_LOOPS 2 	// main loop and self-righting loop

// Electrical //



// Physical //


// Tuned //






/* LOOPER CONSTANTS */

// Set to zero for loop as fast as possible
#define DT_LOOPER 0//0.01							// seconds, 10 milliseconds or 100 Hz

#define TOTAL_NUM_LOOPS 1

/*
 * COMM BUS CONSTANTS
 */

#define I2C_BUS_FREQUENCY 400000		// 400kHz(MPU6050 max)

#define SPI_SCK_FREQUENCY 10000000		// 10MHz (LoRa max)



/* DEBUG CONSTANTS */

//TODO #define USE_DEBUG

#define DEBUG_BAUD_RATE 115200
//2000000




#endif /* CONSTANTS_H_ */
