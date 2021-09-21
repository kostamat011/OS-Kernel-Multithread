/*
 * ientry.cpp
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */
#include"ientry.h"
#include"kevent.h"
#include"control.h"
#include<dos.h>


IVTEntry * IVTEntry::IVTable[256] = {0};

IVTEntry::IVTEntry(unsigned entryNum, pInterrupt routine){
	lockSW
	#ifndef BCC_BLOCK_IGNORE
		oldRout_=getvect(entryNum);
		setvect(entryNum,routine);
	#endif
	myEvent_=new KernelEv(this);
	IVTable[entryNum]=this;
	entryNum_=entryNum;
	unlockSW
}

IVTEntry::~IVTEntry(){
	lockSW
	#ifndef BCC_BLOCK_IGNORE
		setvect(entryNum_,oldRout_);
	#endif
	IVTable[entryNum_]=0;
	delete myEvent_;
	unlockSW
}

void IVTEntry::setMyEvent(KernelEv * ev){
	myEvent_ = ev;
}

void IVTEntry::callOld(){
	oldRout_();
}

KernelEv * IVTEntry::getKernelEvent(){
	return myEvent_;
}


