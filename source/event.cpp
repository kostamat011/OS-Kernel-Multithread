/*
 * event.cpp
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */
#include"control.h"
#include"event.h"
#include"kevent.h"
#include"ientry.h"
#include"pcb.h"
#include<stdio.h>

class KernelEv;


Event::Event(IVTNo ivtNo){
	this->myImpl = IVTEntry::IVTable[ivtNo]->myEvent_;
	this->myImpl->setCreator(PCB::running);
}

void Event::signal(){
	myImpl->signal();
}

Event::~Event(){

}

void Event::wait(){
	// check if creator is running is in KernelEvents wait method
	myImpl->wait();
}




