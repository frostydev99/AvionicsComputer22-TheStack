/*
 * SystemInterface.h
 *
 * Created on: Sep 16, 2021
 * Author: Peter Dentch
 */

#ifndef SRC_SYSTEMINTERFACE_H_
#define SRC_SYSTEMINTERFACE_H_

#include "loops/Looper.h"


/*
 * Abstract/interface class for System objects, these must successfully register at least
 * one Loop to the SystemManager object which contains the Looper object
 */
class SystemInterface {

public:

	SystemInterface(){}
	//	virtual ~SystemInterface(){}

	virtual bool systemInit();
	virtual void registerAllLoops(Looper * runningLooper);


};



#endif /* SRC_SYSTEMINTERFACE_H_ */
