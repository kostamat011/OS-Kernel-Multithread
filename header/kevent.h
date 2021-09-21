/*
 * kevt.h
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */

#ifndef KEVENT_H_
#define KEVENT_H_

class PCB;
class IVTEntry;
class Event;

class KernelEv{
public:
	KernelEv(IVTEntry * iv);
	~KernelEv();
	void signal();
	void wait();
	int val();
	void setCreator(PCB* pcb);
	friend class Event;
private:
	PCB * waitingThread_;
	PCB * creator_;
	IVTEntry *myEntry;
	int val_;
};


#endif /* KEVENT_H_ */
