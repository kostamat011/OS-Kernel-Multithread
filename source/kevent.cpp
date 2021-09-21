/*
 * kevent.cpp
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */
#include"kevent.h"
#include"control.h"
#include"SCHEDULE.H"
#include"pcb.h"
#include"thread.h"
#include<stdio.h>

KernelEv::KernelEv(IVTEntry * iv){
	waitingThread_=0;
	val_=0;
	myEntry = iv;
}

void KernelEv::setCreator(PCB * pcb){
	creator_=pcb;
}

int KernelEv::val(){
	return val_;
}

//destructor
KernelEv::~KernelEv(){

	//unblock all threads waiting on this event
	lockSW
		if(waitingThread_ && waitingThread_->status!=FINISHED){
			waitingThread_->status = READY;
			Scheduler::put(waitingThread_);
		}
	unlockSW
}


void KernelEv::signal(){

	lockSW

		//no blocked thread waiting
		if(!waitingThread_){
			val_=1;
		}

		//unblock waiting thread and set val_ to 0
		else{
			if(waitingThread_ && waitingThread_->status!=FINISHED){
					waitingThread_->status = READY;
					Scheduler::put(waitingThread_);
			}
			waitingThread_=0;
			val_=0;
		}

	unlockSW
}

void KernelEv::wait(){

	//onyl creator of event can wait at event
	if(PCB::running != creator_)
		return;

	lockSW

		//if val is 0 thread needs to wait
		//block thread and call explicit dispatch
		if(!val_){

			waitingThread_=PCB::running;
			PCB::running->status = SUSPENDED;

			unlockSW
			dispatch();
			return;
		}

		//if val is 1 thread can pass and move val to 0
		else{
			val_=0;
		}

	unlockSW
}

