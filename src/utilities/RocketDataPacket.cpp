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

	currentRocketPacket.timestamp = timestamp;
	currentRocketPacket.state = state;

	currentRocketPacket.accelX = accelX;


}




/*
 *
 */
const void* RocketDataPacket::getRocketPacketPtr() {
	return &currentRocketPacket;
}


/*
 *
 */
void RocketDataPacket::setState(uint8_t st) {
	state = st;
}


/*
 *
 */
void RocketDataPacket::setAccelX(int16_t aX) {
	accelX = aX;
}



