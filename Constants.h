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
 * constants like PI, as well as system constants determined through calibration.
 *******************************************************************************/



/*
 * PINS FOR POLARIS PCB
 */
#define E32_LORA_RX	 0		// PCB silkscreen 4
#define E32_LORA_TX  1		// PCB silkscreen 3
#define E32_LORA_M0  2		// PCB silkscreen 25
#define E32_LORA_M1  3		// PCB silkscreen 18
#define E32_LORA_AUX 4		// PCB silkscreen D0


#define PIN_5		 5		// PCB silkscreen D1

#define SPI_FLASH_CS 10		// PCB silkscreen ?













/*
 * COMM BUS SETTINGS
 */

#define I2C_BUS_FREQUENCY 400000		// 400kHz(MPU6050 max)
#define SPI_SCK_FREQUENCY 10000000		// 10MHz (LoRa max)

#define DEBUG_BAUD_RATE 115200		//2000000


/*
 * LOOPER SETTINGS
 */

// Set to zero for loop as fast as possible
#define LOOPER_PERIOD 10 //100						// milliseconds, 10 ms or 100 Hz

// Must be greater than or equal to 1
#define TOTAL_LOOPS 2


/*
 * Use the primary (USB connector) serial interface to send, receive, or stream data.
 *
 * Commenting out this line should always result in the program running properly
 * upon startup, stray Serial.print() statements elsewhere in the code can cause the
 * program to stall without a serial connection present and called before Serial.begin()
 * for Arduino. Teensy USB hardware initialization is performed before setup() runs
 * Program failing to run while printing without serial connection not observed on Teensy 4.0
 */
#define USE_SERIAL						// enable calls to Serial such as print statements


#endif /* CONSTANTS_H_ */
