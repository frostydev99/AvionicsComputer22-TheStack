/*
 * RocketDataPacket.cpp
 *
 * Created on: Apr 18, 2022
 * Author: Peter Dentch
 */

#include "RocketDataPacket.h"

/*
* Constructor for the RocketDataPacket object
*/
RocketDataPacket::RocketDataPacket(){};


/*
 *
 */
void RocketDataPacket::updateRocketPacket() {

	// Rocket packet
	currentRocketPacket.timestamp = timestamp;
	currentRocketPacket.state = state;
	currentRocketPacket.altitude = altitude;
	currentRocketPacket.temperature = temperature;
	currentRocketPacket.accelX = accelX;
	currentRocketPacket.accelY = accelY;
	currentRocketPacket.accelZ = accelZ;
	currentRocketPacket.gyroX = gyroX;
	currentRocketPacket.gyroY = gyroY;
	currentRocketPacket.gyroZ = gyroZ;

	// Telemetry rocket packet
	currentTelemRocketPacket.stateAndTsCombined = stateAndTsCombined;
	currentTelemRocketPacket.altAndTempCombined = altAndTempCombined;


}


/*
 *
 */
void RocketDataPacket::updateFromTelemPacket() {

//	stateAndTsCombined = currentTelemRocketPacket.stateAndTsCombined;
//	altAndTempCombined = currentTelemRocketPacket.altAndTempCombined;

	// State and Time stamp parsing
	// TODO actually include state...
	currentTelemRocketPacket.stateAndTsCombined = timestamp;

	// Altitude and Temperature parsing
	uint8_t * altAndTempPtr = (uint8_t *) &altAndTempCombined;

	uint8_t altitudeMSB = altAndTempPtr[3];
	uint8_t altitudeCSB = altAndTempPtr[2];
	uint8_t altitudeLSB = altAndTempPtr[1] & B11110000;	// keep only first 4 bits

	uint8_t temperatureMSB = (altAndTempPtr[1] << 4)  |  (altAndTempPtr[0] >> 4);
	uint8_t temperatureLSB = (altAndTempPtr[0] << 4);

	rawToAltitude(altitudeMSB, altitudeCSB, altitudeLSB);	// sets altitude in function
	rawToTemperature(temperatureMSB, temperatureLSB);		// sets temperature in function

	currentTelemRocketPacket.altAndTempCombined = altAndTempCombined;

	// IMU readings
//	accelX = currentTelemRocketPacket.accelX;
//	accelY = currentTelemRocketPacket.accelY;
//	accelZ = currentTelemRocketPacket.accelZ;
//	gyroX = currentTelemRocketPacket.gyroX;
//	gyroY = currentTelemRocketPacket.gyroY;
//	gyroZ = currentTelemRocketPacket.gyroZ;

	currentTelemRocketPacket.accelX = accelX;
	currentTelemRocketPacket.accelY = accelY;
	currentTelemRocketPacket.accelZ = accelZ;
	currentTelemRocketPacket.gyroX = gyroX;
	currentTelemRocketPacket.gyroY = gyroY;
	currentTelemRocketPacket.gyroZ = gyroZ;


}


/*
 * From the raw sensor registers, converts the altitude to a float. The sensor is in altitude mode
 * and returns the raw data as a Q16.4 signed fixed point.
 */
float RocketDataPacket::rawToAltitude(uint8_t msb, uint8_t csb, uint8_t lsb) {
	float altitude = 0.0;

	// Integer part of the altitude reading
	int16_t integerPart = (msb << 8) | csb;
	// Fractional part of the altitude reading
	uint8_t fractionalPart = (lsb >> 4);

	altitude = integerPart + (fractionalPart * 0.0625);

	this->altitude = altitude;
	return altitude;
}

/*
 * Converting temperature raw data to a float. The sensor register data is a raw
 * Q8.4 signed fixed point
 */
float RocketDataPacket::rawToTemperature(uint8_t msb, uint8_t lsb) {

	float temperature = 0.0;
	int8_t integerPart = msb;

	// Get fractional part
	uint8_t fractionalPart = ((lsb & B11110000) >> 4);

	// Integer and fractional parts combined
	temperature = integerPart + (fractionalPart * 0.0625);

	this->temperature = temperature;

	return temperature;

}



/*
 *
 */
const void* RocketDataPacket::getRocketPacketPtr() {
	return &currentRocketPacket;
}

const void* RocketDataPacket::getTelemRocketPacketPtr() {
	return &currentTelemRocketPacket;
}

uint8_t RocketDataPacket::getState() {
	return state;
}

float RocketDataPacket::getAltitude() {
	return altitude;
}

float RocketDataPacket::getTemperature() {
	return temperature;
}

int16_t RocketDataPacket::getAccelX() {
	return accelX;
}

int16_t RocketDataPacket::getAccelY() {
	return accelY;
}

int16_t RocketDataPacket::getAccelZ() {
	return accelZ;
}

int16_t RocketDataPacket::getGyroX() {
	return gyroX;
}

int16_t RocketDataPacket::getGyroY() {
	return gyroY;
}

int16_t RocketDataPacket::getGyroZ() {
	return gyroZ;
}

void RocketDataPacket::setState(uint8_t st) {
	state = st;
}

void RocketDataPacket::setStateAndTsCombined(uint32_t combinedRaw) {
	stateAndTsCombined = combinedRaw;
}

void RocketDataPacket::setAltitude(float alt) {
	altitude = alt;
}

void RocketDataPacket::setTemperature(float temp) {
	temperature = temp;
}

void RocketDataPacket::setAltAndTempCombined(uint32_t combinedRaw) {
	altAndTempCombined = combinedRaw;
}


void RocketDataPacket::setAccelX(int16_t aX) {
	accelX = aX;
}

void RocketDataPacket::setAccelY(int16_t aY) {
	accelY = aY;
}

void RocketDataPacket::setAccelZ(int16_t aZ) {
	accelZ = aZ;
}

void RocketDataPacket::setGyroX(int16_t gX) {
	gyroX = gX;
}

void RocketDataPacket::setGyroY(int16_t gY) {
	gyroY = gY;
}

void RocketDataPacket::setGyroZ(int16_t gZ) {
	gyroZ = gZ;
}


