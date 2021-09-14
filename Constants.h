/*
 * Constants.h
 *
 * Created on: Sep 12, 2021
 * Author: Peter Dentch
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/*******************************************************************************
 * A list of constants used by the robot computer board code. Includes physics
 * constants as well as constants determined through calibration.
 *******************************************************************************/

/*
 * Use the primary (USB connector) serial interface to send, receive, or stream data.
 *
 * Commenting out this line should always result in the program running properly
 * upon startup, stray Serial.print() statements elsewhere in the code can cause the
 * program to stall without a serial connection present if called without Serial.begin()
 */
//#define USE_SERIAL


/* LOOPER CONSTANTS */

// Set to zero for loop as fast as possible
#define DT_LOOPER 0.1//0.01							// seconds, 10 milliseconds or 100 Hz
#define TOTAL_NUM_LOOPS 1


/* COMM BUS CONSTANTS */

#define SERIAL_BAUD_RATE  115200		// 115200 baud (bits/s)
#define I2C_BUS_FREQUENCY 400000		// 400kHz(MPU6050 max)
#define SPI_SCK_FREQUENCY 10000000		// 10MHz (LoRa max)



/* ROBOT CONSTANTS */

//#define ROBOT_NUM_LOOPS 2 	// main loop, drivetrain loop

// Electrical //



// Physical //


// Tuned //





#endif /* CONSTANTS_H_ */
