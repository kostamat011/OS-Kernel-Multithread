/*
 * thread.cpp
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */
#include "thread.h"
#include "control.h"
#include "pcb.h"
#include "inter.h"
#include "SCHEDULE.H"
#include "list.h"
#include "idleThr.h"
#include "sighand.h"
#include"hnd_lst.h"
#include"sgn_lst.h"
#include <stdio.h>
#include <stdlib.h>

volatile int dispatchExplicit=0;

//constructor
Thread::Thread(StackSize stackSize, Time timeSlice){
	myPCB = new PCB(timeSlice,stackSize,this);
}

Thread::Thread(PCB * pcb){
	myPCB=pcb;
	myPCB->myThread_=this;
}

//destructor
Thread::~Thread(){
	 waitToComplete();
	 lockSW
	 	 delete myPCB;
	 unlockSW
}

void dispatch(){
	#ifndef BCC_BLOCK_IGNORE
		lockINTR
	#endif
	dispatchExplicit=1;
	timer();
	#ifndef BCC_BLOCK_IGNORE
		unlockINTR
	#endif
}

void Thread::waitToComplete(){
	lockSW
	if((myPCB->status!= FINISHED) && (this->myPCB!=PCB::running))

	{
		//block running thread
		//and add it to waiting list of this thread

		PCB::running->status = SUSPENDED;
		myPCB->waitingList->push_back(PCB::running);

		unlockSW

		//explicit dispatch
		dispatch();

		return; //to avoid double unlock
	}
	unlockSW

}

void Thread::start(){

	if(myPCB->status != INIT)
		return;

	lockSW
		myPCB->status = READY;
		Scheduler::put(myPCB);
	unlockSW
}

ID Thread::getId(){
	return this->myPCB->getPid();
}

ID Thread::getRunningId(){
	return PCB::running->getPid();
}

Thread * Thread::getThreadById(ID id){
	Node<PCB> *tmp = PCB::allPCB->get_head();
	while(tmp){
		if(tmp->info->getPid()==id)
			return tmp->info->getThread();
		tmp=tmp->next;
	}
	return 0;
}

/*******************************signals**************************/

void Thread::signal(SignalId signal){
	if(signal==1 || signal==2)
		return;
	myPCB->signals->insertSignal(signal);
}

//system signal1
void Thread::signal1(){
	myPCB->signals->insertSignal(1);
}


//system signal 2
void Thread::signal2(){
	myPCB->signals->insertSignal(2);
}


void Thread::registerHandler(SignalId signal,SignalHandler handler){
	myPCB->handlers[signal]->insertHandler(handler);
}

void Thread::blockSignal(SignalId signal){
	myPCB->blockedSignals[signal] = 1;
}

void Thread::blockSignalGlobally(SignalId signal){
	PCB::globalBlockedSignals[signal] = 1;
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	HandlerNode *a = 0, *b = 0;
	HandlerNode * tmp = myPCB->handlers[id]->head;

	while(tmp){
		if(tmp->handler==hand1)
			a=tmp;

		if(tmp->handler==hand2)
			b=tmp;

		tmp=tmp->next;
	}

	if( (!a) || (!b) )
		return;

	a->handler=hand2;
	b->handler=hand1;

}

void Thread::unblockSignal(SignalId signal){
	myPCB->blockedSignals[signal] = 0;
}

void Thread::unblockSignalGlobally(SignalId signal){
	PCB::globalBlockedSignals[signal] = 0;
}

void Thread::unregisterAllHandlers(SignalId id){
	lockSW
	delete myPCB->handlers[id];
	unlockSW

}


