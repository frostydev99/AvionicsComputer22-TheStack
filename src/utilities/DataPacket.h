/*
 * DataPacket.h
 *
 * Created on: Apr 18, 2022
 * Author: Peter Dentch
 */

#ifndef SRC_UTILITIES_DATAPACKET_H_
#define SRC_UTILITIES_DATAPACKET_H_

#include "Arduino.h"


/*
 * DataPacket abstract class
 * A system will not typically log only a time stamp
 */
class DataPacket {

private:

	/*
	 * Data packet structure for logging, transmitting, and printing for the user
	 * Size is 4 bytes
	 * 4 for time stamp
	 */
	struct Packet {
		uint32_t timestamp;
	} currentPacket;


protected:

	uint32_t timestamp = 0;

	void updatePacket();


public:

	DataPacket();
	//	~DataPacket(){}

	const void* getPacketPtr();

	uint32_t getTimestamp();

	void setTimestamp(uint32_t ts);


};


#endif /* SRC_UTILITIES_DATAPACKET_H_ */
