/*
 * pcb.cpp
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */
#include"pcb.h"
#include"control.h"
#include"SCHEDULE.H"
#include"idleThr.h"
#include"thread.h"
#include"list.h"
#include"sighand.h"
#include"hnd_lst.h"
#include"sgn_lst.h"
#include"ksem.h"
#include<stdio.h>
#include<dos.h>


extern PCB * mainPCB;
extern IdleThread * idleT;

	//static members initialization
	PCB * PCB::running=0;
	ID PCB::spid=0;
	List<PCB>* PCB::allPCB = new List<PCB>();
	int PCB::globalBlockedSignals[MAX_SIGNAL_NUM];

	void PCB::wrapper(){

		//do thread work
		running->myThread_->run();
		//thread is finished

		lockSW
			running->status=FINISHED;

			//unblock all waiting threads
			Node<PCB> * tmp = 0;
			Node<PCB> * curr = running->waitingList->pop_head();
			while(curr){

				if(curr->info->status== SUSPENDED){
					curr->info->status = READY;
					Scheduler::put(curr->info);
				}
				tmp=curr;
				curr=running->waitingList->pop_head();
				delete tmp;
			}


			if(running->parent)
				running->parent->myThread_->signal1();
			if(running!=idleT->idlePCB())
				running->myThread_->signal2();
			running->handleSignals();


		unlockSW

		dispatch();
	}

	//constructor
	PCB::PCB(Time timeSlice, StackSize stackSize, Thread * T){
		if (stackSize > MAX_STACK_SIZE)
			stackSize = MAX_STACK_SIZE;

		size_ = stackSize / sizeof(unsigned);
		timeSlice_ = timeSlice;
		status = INIT;
		pid_ = spid++;
		myThread_=T;
		waitingTime_=-1;
		semStatus=0;
		parent = running; //parent is null for main
		waitingList=new List<PCB>();


		/*************************STACK INIT**************************/
		#ifndef BCC_BLOCK_IGNORE
			lockINTR
		#endif
		stack_ = new unsigned [size_];
		#ifndef BCC_BLOCK_IGNORE
			stack_[size_-1]=0x200; //set PSWI
			stack_[size_-2]=FP_SEG(PCB::wrapper); //set CS
			stack_[size_-3]=FP_OFF(PCB::wrapper); //set IP
			ss_ = FP_SEG(stack_ + size_ - 12);
			sp_ = FP_OFF(stack_ + size_ - 12);
			bp_ = sp_;
			unlockINTR
		#endif
		/*************************************************************/

		/*************************SIGNALS**************************/
		signals = new SignalList();
		for(int i=0; i<MAX_SIGNAL_NUM; ++i){
			blockedSignals[i]=0;
			handlers[i] = new HandlerList();

			//main pcb gets only handler 0
			if(this->getPid()==0){
				if(i==0){
					handlers[i]->insertHandler(SignalHandler0);
				}
			}

			//every other pcb inherits handlers from parent
			else{
				handlers[i] = parent->handlers[i]->copy();
			}
		}
		/*************************************************************/


		//add PCB to all pcb list
		allPCB->push_back(this);
	}

	//destructor
	PCB::~PCB(){
		lockSW
			allPCB->remove_byInfo(this);
			if(waitingList)delete waitingList;
			if(stack_)delete[] stack_;
			if(signals)delete signals;
			for(int i=0; i<MAX_SIGNAL_NUM;++i)
				if(handlers[i])delete handlers[i];
		unlockSW
	}

	void PCB::initGlobalSignalFlags(){
		for(int i=0; i<MAX_SIGNAL_NUM; ++i){
			globalBlockedSignals[i]=0;
		}
	}

	//pid getter
	ID PCB::getPid(){
		return this->pid_;
	}

	//timeSlice getter
	Time PCB::getTime(){
		return this->timeSlice_;
	}

	//sp setter
	void PCB::setSp(unsigned sp){
		sp_ = sp;
	}

	//sp getter
	unsigned PCB::getSp(){
		return sp_;
	}

	//ss setter
	void PCB::setSs(unsigned ss){
		ss_=ss;
	}

	//ss getter
	unsigned PCB::getSs(){
		return ss_;
	}

	//bp setter
	void PCB::setBp(unsigned bp){
		bp_=bp;
	}

	//bp getter
	unsigned PCB::getBp(){
		return bp_;
	}

	//myThread getter
	Thread * PCB::getThread(){
		return myThread_;
	}

	//infinite getter
	int PCB::isInfinite(){
		return (timeSlice_==0)?1:0;
	}

	int PCB::getWaitingTime(){
		return waitingTime_;
	}

	void PCB::setWaitingTime(int t){
		waitingTime_=t;
	}

	void PCB::handleSignals(){

		if(running->getPid()==1)
			return;


		lockSW
			SignalNode * curr =signals->head;
			while(curr){

				//if signal is not blocked or global blocked
				//run all handlers for it and remove it from list

				SignalId currId = curr->signal;
				if(!blockedSignals[currId]
				&& !globalBlockedSignals[currId])
				{
					HandlerNode * currHandler = handlers[currId]->head;
					while(currHandler){
						currHandler->handler();
						currHandler=currHandler->next;
					}
					curr=curr->next;
					signals->removeSignal(currId);
				}
				else{
					curr=curr->next;
				}
			}
		unlockSW
	}

	void PCB::SignalHandler0(){

	running->status=FINISHED;
	if(running->parent)
			running->parent->myThread_->signal1();
	if(running!=idleT->idlePCB())
			running->myThread_->signal2();

	//unblock all waiting threads
	Node<PCB> * tmp = 0;
	Node<PCB> * curr = running->waitingList->pop_head();
	while(curr){

		if(curr->info->status== SUSPENDED){
			curr->info->status = READY;
			Scheduler::put(curr->info);
		}
		tmp=curr;
		curr=running->waitingList->pop_head();
		delete tmp;
	}

	delete running->signals; running->signals=0;
	delete running->waitingList;running->waitingList=0;
	for(int i=0; i<MAX_SIGNAL_NUM; ++i)
	{
		delete running->handlers[i];
		running->handlers[i] = 0;
	}

	if(lock==1) lock=0; //complete unlock from handle signals method

	dispatch();

	}
