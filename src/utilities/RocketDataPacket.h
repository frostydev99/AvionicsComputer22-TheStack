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
 *	Different packet forms for optimization like minimal data to transmit
 */
class RocketDataPacket : public DataPacket {

//private:
public:

	/*
	 * Rocket packet
	 * Standard Rocket data packet structure for logging/transmitting/printing for the user
	 * sizeof(currentRocketPacket) = 28 bytes
	 */
	struct RocketPacket {
		uint32_t timestamp;		// bytes 0 - 3
		uint8_t state;			// byte 4 (5 - 7 go unused despite declared as 1)
		float altitude;			// bytes 8 - 11
		float temperature;		// bytes 12 - 15
		int16_t accelX;			// bytes 16 & 17
		int16_t accelY;
		int16_t accelZ;
		int16_t gyroX;
		int16_t gyroY;
		int16_t gyroZ;

	} currentRocketPacket;


	/*
	 * Telemetry rocket packet
	 * Compressed data packet structure for transmitting
	 * Combines state (4 bits) and timestamp (28 bits) readings into 4 bytes
	 * Combines altitude (20 bits) and temperature (12 bits) readings into 4 bytes
	 * sizeof(currentTelemRocketPacket) = 20 bytes
	 */
	struct TelemRocketPacket {
		uint32_t stateAndTsCombined;
		uint32_t altAndTempCombined;
		int16_t accelX;
		int16_t accelY;
		int16_t accelZ;
		int16_t gyroX;
		int16_t gyroY;
		int16_t gyroZ;

	} currentTelemRocketPacket;


	// Rocket packet
	uint8_t state 		= 0;
	float altitude 		= 0;
	float temperature 	= 0;
	int16_t accelX 		= 0;
	int16_t accelY 		= 0;
	int16_t accelZ 		= 0;
	int16_t gyroX 		= 0;
	int16_t gyroY 		= 0;
	int16_t gyroZ 		= 0;

	// Telemetry rocket packet
	uint32_t stateAndTsCombined = 0;
	uint32_t altAndTempCombined = 0;


	// See peripherals/MPL3115A2.h
	float rawToAltitude(uint8_t msb, uint8_t csb, uint8_t lsb);
	float rawToTemperature(uint8_t msb, uint8_t lsb);


public:

	RocketDataPacket();
	//	~RocketDataPacket(){}

	//void updateFromRocketPacket();
	void updateFromTelemPacket();
	//void updateFromShortTelemPacket();

	void updateRocketPacket();  // TODO Should be private, or not exist? Used in setters?
								// Update ALL data packets?


	// Are these unneeded when using the struct pointer?
	void setRocketPacket(const void *packet);
	void setRocketTelemPacket(const void *packet);
	void setRocketShortTelemPacket(const void *packet);

	const void* getRocketPacketPtr();
	const void* getTelemRocketPacketPtr();

	uint8_t getState();
	float getAltitude();
	float getTemperature();
	int16_t getAccelX();
	int16_t getAccelY();
	int16_t getAccelZ();
	int16_t getGyroX();
	int16_t getGyroY();
	int16_t getGyroZ();

	void setState(uint8_t st);
	void setStateAndTsCombined(uint32_t combinedRaw);
	void setAltitude(float alt);
	void setTemperature(float temp);
	void setAltAndTempCombined(uint32_t combinedRaw);
	void setAccelX(int16_t aX);
	void setAccelY(int16_t aY);
	void setAccelZ(int16_t aZ);
	void setGyroX(int16_t gX);
	void setGyroY(int16_t gY);
	void setGyroZ(int16_t gZ);


};


#endif /* SRC_UTILITIES_ROCKETDATAPACKET_H_ */
