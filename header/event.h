/*
 * event.h
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */
#include "ientry.h"
#include "kevent.h"

#ifndef EVENT_H_
#define EVENT_H_

typedef unsigned char IVTNo;
class KernelEv;
class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
private:
	KernelEv* myImpl;
};



#endif /* EVENT_H_ */
