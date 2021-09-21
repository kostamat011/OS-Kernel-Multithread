/*
 * ksem.cpp
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */

#include"control.h"
#include"pcb.h"
#include"semaphor.h"
#include"thread.h"
#include"ksem.h"
#include"SCHEDULE.H"
#include<stdio.h>
#include"list.h"


volatile int updateLock=0;


List<KernelSem> * KernelSem::allKernelSem = new List<KernelSem>();
int KernelSem::ssid = 0;

//constructor
KernelSem::KernelSem(int val){
	sid_=ssid++;
	val_=val;
	blockedList = new List<PCB>();
	allKernelSem->push_back(this);
}

//destructor
KernelSem::~KernelSem(){
	lockSW
		delete blockedList;
		allKernelSem->remove_byInfo(this);
	unlockSW
}

//static method called on every timer tick
void KernelSem::updateAllKernelSem(){

	//new update of semaphores cannot start until current is finished
	updateLock=1;

	//loop trough all semaphores
	Node<KernelSem> * tmp = allKernelSem->get_head();
	while(tmp){

		//if it has any thread in its blocked list update it
		if(tmp->info->blockedList->get_head()){
			tmp->info->updateBlockedList();
		}

		tmp=tmp->next;
	}

	updateLock=0;
}

int KernelSem::signal(int n){
	lockSW

	if(n==0) //unblock one thread
	{
		//increase val and if there is waiting thread unblock it
		if(val_++<0){

			//get current head and remove it from list
			Node<PCB> * tmp = blockedList->pop_head();

			//set flag that pcb is unblocked by signal not by time
			tmp->info->semStatus=1;

			//put waiting pcb in scheduler
			tmp->info->status=READY;
			Scheduler::put(tmp->info);

			//delete old head from blockedList
			delete tmp;
		}
		unlockSW
		return 0;
	}

	else if(n>0) //unblock max n threads and return count of unblocked
	{
		//increment val
		val_+=n;

		//free n waiting pcbs or all if less than n are in list

		Node<PCB> *curr = blockedList->get_head();
		int cnt = 0; // count of unblocked pcbs
		while(cnt<n && curr){

			//set flag that pcb is unblocked by signal not by time
			curr->info->semStatus=1;

			//put waiting pcb in scheduler
			curr->info->status = READY;
			Scheduler::put(curr->info);

			//move curr to next and remove current head from list
			curr=curr->next;
			Node<PCB> *tmp = blockedList->pop_head();
			delete tmp;

			//increase unblocked count
			++cnt;
		}
		unlockSW
		return cnt; //return number of unblocked pcbs
	}

	else{ //do nothing and return n

		unlockSW
		return n;
	}
}


int KernelSem::wait(Time maxTimeToWait){
	lockSW
		//if decremented val is <0 thread needs to wait
		if(val_--<=0)
		{
			// unlimited waiting
			if(maxTimeToWait==0){
				PCB::running->setWaitingTime(-1);
			}

			//limited waiting
			else{
				PCB::running->setWaitingTime(maxTimeToWait);
			}

			//suspend and put in blocked list and switch context
			PCB::running->status = SUSPENDED;
			this->blockedList->push_back(PCB::running);

			unlockSW
			dispatch();
		}
		else
			unlockSW

	return PCB::running->semStatus;
}



void KernelSem::updateBlockedList(){

	//unblock all limited waiting PCBs where time reached 0
	lockSW

		Node<PCB> * curr = this->blockedList->get_head();
		while(curr){
			if(curr->info->getWaitingTime()!= (-1) ) //-1 are pcbs with unlimited time
			{
				curr->info->setWaitingTime(curr->info->getWaitingTime() - 1);
				if((curr->info->getWaitingTime()) <= 0){

					//unblock thread
					if(curr->info->status==SUSPENDED){
						curr->info->status=READY;
						Scheduler::put(curr->info);
					}

					//remove current from blockedList and move to next
					PCB * toRemove = curr->info;
					curr->info->semStatus=0; // set flag unblocked by time
					curr=curr->next;
					blockedList->remove_byInfo(toRemove);
					//increase val for 1 thread unblocked
					val_++;
				}

				else{ //waiting is not unlimited but have more time
					curr=curr->next;
				}

			}

			else{ //unlimited waiting
				curr = curr->next;
			}
		}

	unlockSW
}

int KernelSem::getVal(){
	return val_;
}

int KernelSem::getSid(){
	return sid_;
}


