/*
 * inter.cpp
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */
#include "control.h"
#include "inter.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include <stdio.h>
#include "idleThr.h"
#include "ksem.h"
#include"list.h"

volatile int counter=0;
volatile unsigned tsp=0;
volatile unsigned tss=0;
volatile unsigned tbp=0;

extern IdleThread * idleT;
extern PCB * mainPCB;
extern void tick();
extern volatile int lock;

void interrupt timer(){
	//update counter if current thread is not infinite and timer was called by time interrupt
	if( !PCB::running->isInfinite() && !dispatchExplicit )
		counter--;

	if(   ( (!lock) && (counter<=0) && (!PCB::running->isInfinite()) ) || (dispatchExplicit)){
		//do context switch

		//save context of running thread
		#ifndef BCC_BLOCK_IGNORE
		asm{
			mov tsp, sp;
			mov tss, ss;
			mov tbp, bp;
		}
		#endif
		PCB::running->setSp(tsp);
		PCB::running->setSs(tss);
		PCB::running->setBp(tbp);
		//put running in scheduler
		//idle thread never goes in scheduler
		if(PCB::running->status == RUNNING){

			PCB::running->status = READY;
			Scheduler::put(PCB::running);
		}

		//take new running from scheduler
		//if scheduler is empty use idlePCB
		PCB::running = Scheduler::get();

		if(!PCB::running){
			PCB::running=idleT->idlePCB();
		}
		else
			PCB::running->status = RUNNING;

		//update time counter to timeSlice of new runningPCB
		counter = PCB::running->getTime();

		//restore new running context
		tsp=PCB::running->getSp();
		tss=PCB::running->getSs();
		tbp=PCB::running->getBp();
		#ifndef BCC_BLOCK_IGNORE
		asm{
			mov sp, tsp;
			mov ss, tss;
			mov bp, tbp;
		}
		#endif

		PCB::running->handleSignals();
	}

	// if timer was called by time interrupt dispatchExplicit is 0
	if(!dispatchExplicit){

		if(!updateLock){
			KernelSem::updateAllKernelSem(); //update all sempahores
		}

		tick(); //defined in usermain

		#ifndef BCC_BLOCK_IGNORE
			asm int 60h; //call old timer routine on IVTP 60
		#endif
	}
	else //if timer was called explicit dispatchExplicit is 1
		dispatchExplicit=0;
}
