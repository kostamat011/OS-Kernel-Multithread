/*
 * ientry.h
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */

#ifndef IENTRY_H_
#define IENTRY_H_


typedef void interrupt (*pInterrupt)(...);
class KernelEv;
class Event;

class IVTEntry{
public:
	static IVTEntry * IVTable[256];
	IVTEntry(unsigned entryNum, pInterrupt routine);
	~IVTEntry();
	void setMyEvent(KernelEv * ev);
	void callOld();
	KernelEv * getKernelEvent();
	friend class Event;
private:
	KernelEv * myEvent_;
	pInterrupt oldRout_;
	int entryNum_;
};

#define PREPAREENTRY(numEntry,callOldpp)\
	void interrupt inter##numEntry(...);\
	IVTEntry newEntry##numEntry(numEntry,inter##numEntry);\
	void interrupt inter##numEntry(...){\
		newEntry##numEntry.getKernelEvent()->signal();\
		if(callOldpp==1)\
		newEntry##numEntry.callOld();\
	}


#endif /* IENTRY_H_ */
