/*
 * DataPacket.cpp
 *
 * Created on: Apr 18, 2022
 * Author: Peter Dentch
 */

#include "DataPacket.h"

/*
* Constructor for the DataPacket object
*/
DataPacket::DataPacket(){};


/*
 *
 */
void DataPacket::updatePacket() {}


/*
 *
 */
const void* DataPacket::getPacketPtr() {
	return &currentPacket;
}


/*
 * Getter for the time stamp
 */
uint32_t DataPacket::getTimestamp() {
	return timestamp;
}

/*
 * Setter for the time stamp
 */
void DataPacket::setTimestamp(uint32_t ts) {
	timestamp = ts;
}




