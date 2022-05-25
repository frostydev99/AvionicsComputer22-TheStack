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

enum BoardStates {
	IDLE,
	INITIALIZE,
	SENDING
};


/*
 * PINS FOR POLARIS PCB
 */
#define E32_LORA_TX  0		// PCB silkscreen 3	(MCU Rx pin)
#define E32_LORA_RX  1		// PCB silkscreen 4 (MCU Tx pin)
#define E32_LORA_M0  2		// PCB silkscreen 25
#define E32_LORA_M1  3		// PCB silkscreen 18
#define E32_LORA_AUX 4		// PCB silkscreen D0


#define PIN_5		 5		// PCB silkscreen D1

#define SPI_FLASH_CS 10		// PCB silkscreen ?

/*
 * PINS FOR POWER BOARD
 */
#define PB_SERVO_DISABLE A2
#define PB_SERVO_PWM_1 5
#define PB_SERVO_PWM_2 6
#define PB_SERVO_PWM_3 9
#define PB_SERVO_PWM_4 10
#define PB_VCC A3
#define PB_7V A5
#define PB_MCP515 8

/*
 * POWER BOARD CONFIG
 */
#define VOLTAGE_READ_INTERVAL 1000
#define VCC_REGRESSION_SLOPE 11.74825
#define VCC_REGRESSION_INTERCEPT 153.5667

#define SERVO1_RANGE_LOWER 0
#define SERVO1_RANGE_UPPER 360
#define SERVO1_LIMIT_LOWER 0
#define SERVO1_LIMIT_UPPER 360

#define SERVO2_RANGE_LOWER 0
#define SERVO2_RANGE_UPPER 360
#define SERVO2_LIMIT_LOWER 0
#define SERVO2_LIMIT_UPPER 360

#define SERVO3_RANGE_LOWER 0
#define SERVO3_RANGE_UPPER 360
#define SERVO3_LIMIT_LOWER 0
#define SERVO3_LIMIT_UPPER 360

#define SERVO4_RANGE_LOWER 0
#define SERVO4_RANGE_UPPER 360
#define SERVO4_LIMIT_LOWER 0
#define SERVO4_LIMIT_UPPER 360

/*
 * CAN MESSAGE IDS/DLCS
 * In order of decreasing priority, maximum is 2048
 */
#define CAN_CHARGE_FIRE 500
#define DLC_CHARGE_FIRE 2
// [1: {uint_8} CHARGE ID] [2: {bool} ENABLE]

#define CAN_SERVO_ACTUATE 501
#define DLC_SERVO_ACTUATE 3
// [1: {uint_8} SERVO ID] [2-3: {uint_16} ANGLE]

#define CAN_SERVO_POWER 1500
#define DLC_SERVO_POWER 1
// [1: {bool} SERVO POWER ENABLE]

#define CAN_VOLTAGE_READ 1700
#define DLC_VOLTAGE_READ 4
// [1-2: {uint_16} MAIN BATTERY VOLTAGE (mV)] // planned [3-4: {uint_16} REGULATED SERVO VOLTAGE (mV)]

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
#define TOTAL_LOOPS 1


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
