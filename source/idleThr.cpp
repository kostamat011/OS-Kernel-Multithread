/*
 * idle_thread.cpp
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */


#include "idleThr.h"
#include "control.h"
#include "pcb.h"
#include "thread.h"
#include  <stdio.h>


IdleThread::IdleThread():Thread(defaultStackSize,1){
	runIdle=1;
}

void IdleThread::run(){
	while(runIdle);
}

void IdleThread::start(){
	myPCB->status=READY;
}

void IdleThread::stop(){
	runIdle=0;
}

PCB * IdleThread::idlePCB(){
	return myPCB;
}


