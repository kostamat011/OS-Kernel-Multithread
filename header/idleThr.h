/*
 * idle_thread.h
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */

#ifndef IDLETHR_H_
#define IDLETHR_H_
#include"thread.h"

// idle thread does nothing and is set on running when scheduler is empty
class IdleThread:public Thread{
public:
	IdleThread();
	virtual ~IdleThread(){};
	PCB *idlePCB();
	void run();
	void start();
	void stop();
private:
	volatile int runIdle;
};


#endif /* IDLETHR_H_ */
