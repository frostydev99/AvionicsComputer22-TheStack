/*
 * SubsystemInterface.h
 *
 * Interface structure for a subsystem
 *
 * Created on: Dec 19, 2020
 * Author: Peter Dentch
 */

#ifndef SRC_SUBSYSTEMS_SUBSYSTEMINTERFACE_H_
#define SRC_SUBSYSTEMS_SUBSYSTEMINTERFACE_H_

#include "../loops/Looper.h"


/*
 * Abstract/interface class for subsystem objects, which register loops (periodic tasks)
 * and contain peripherals/utilities as fields
 */
class SubsystemInterface {

public:

	SubsystemInterface(){}
//		virtual ~SubsystemInterface(){}

	virtual bool subsystemInit() = 0;
	virtual void zeroSensors() = 0;
	virtual void registerLoops(Looper * runningLooper) = 0;
	virtual void printOutput() = 0;


};


#endif /* SRC_SUBSYSTEMS_SUBSYSTEMINTERFACE_H_ */
