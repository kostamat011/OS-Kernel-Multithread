/*
 * semaphor.cpp
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */
#include"ksem.h"
#include"semaphor.h"
#include"control.h"

Semaphore::Semaphore(int init){
	myImpl = new KernelSem(init);
}

Semaphore::~Semaphore(){
	lockSW
		delete myImpl;
	unlockSW
}

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n){
	return myImpl->signal(n);
}

int Semaphore::val() const{
	return myImpl->getVal();
}

