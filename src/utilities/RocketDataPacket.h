/*
 * RocketDataPacket.h
 *
 * Created on: Apr 18, 2022
 * Author: Peter Dentch
 */

#ifndef SRC_UTILITIES_ROCKETDATAPACKET_H_
#define SRC_UTILITIES_ROCKETDATAPACKET_H_

#include "Arduino.h"

#include "DataPacket.h"


/*
 *	Rocket DataPacket class
 *	Includes sensor readings from the barometer and IMU
 *	Different packet forms for functionality like minimal data to transmit
 */
class RocketDataPacket : public DataPacket {

private:

	/*
	 * Rocket data packet structure for logging, transmitting, and printing for the user
	 * Size is 28 bytes
	 * 4 for time stamp
	 * 4 for state (despite declared as 1)
	 * 4 for altitude
	 * 4 for temperature
	 * 2 for acceleration x
	 * 2 for acceleration y
	 * 2 for acceleration z
	 * 2 for angular rate x
	 * 2 for angular rate y
	 * 2 for angular rate z
	 */
	struct RocketPacket {
		uint32_t timestamp;		// bytes 0 - 3
		uint8_t state;			// byte 4 (5 - 7 go unused)
		float altitude;			// bytes 8 - 11
		float temperature;		// bytes 12 - 15
		int16_t accelX;			// bytes 16 & 17
		int16_t accelY;
		int16_t accelZ;
		int16_t gyroX;
		int16_t gyroY;
		int16_t gyroZ;

	} currentRocketPacket;


	uint8_t state 		= 0;
	float altitude 		= 0;
	float temperature 	= 0;
	int16_t accelX 		= 0;
	int16_t accelY 		= 0;
	int16_t accelZ 		= 0;
	int16_t gyroX 		= 0;
	int16_t gyroY 		= 0;
	int16_t gyroZ 		= 0;



	//void updateRocketPacket();
	//void updateTelemPacket();
	//void updateShortTelemPacket();



public:

	RocketDataPacket();
	//	~RocketDataPacket(){}

	void updateRocketPacket();  // TODO Should be private, or not exist? Used in setters?

	const void* getRocketPacketPtr();

	uint8_t getState();
	float getAltitude();
	float getTemperature();
	int16_t getAccelX();
	int16_t getAccelY();
	int16_t getAccelZ();
	int16_t getGyroX();
	int16_t getGyroY();
	int16_t getGyroZ();

	void setRocketPacket(const void *packet);
	void setRocketTelemPacket(const void *packet);
	void setRocketShortTelemPacket(const void *packet);

	void setState(uint8_t st);
	void setAltitude(float alt);
	void setTemperature(float temp);
	void setAccelX(int16_t aX);
	void setAccelY(int16_t aY);
	void setAccelZ(int16_t aZ);
	void setGyroX(int16_t gX);
	void setGyroY(int16_t gY);
	void setGyroZ(int16_t gZ);


};


#endif /* SRC_UTILITIES_ROCKETDATAPACKET_H_ */
