/*
 * pcb.h
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

typedef enum State{INIT, READY, RUNNING, SUSPENDED, FINISHED};
typedef unsigned long StackSize;
typedef unsigned int Time; // time, x 55ms
typedef int ID;

#define MAX_SIGNAL_NUM 16
#define MAX_STACK_SIZE 65536

class Thread;
class IdleThread;

typedef unsigned SignalId;
typedef void (*SignalHandler)();
class SignalHandlerNode;
class KernelSem;
class HandlerList;
class SignalList;

template<class T>
class List;


class PCB{
public:
	static PCB * running;
	static List<PCB> * allPCB;
	static int globalBlockedSignals[MAX_SIGNAL_NUM];
	static void initGlobalSignalFlags();
	State status;
	int semStatus; // 0 if released by time , 1 if released by signal
	PCB * parent; //pcb that was running when this pcb was created
	List<PCB> *waitingList; // all threads waiting for this to complete


	//constructor destructor wrapper
	PCB(Time timeSlice, StackSize stackSize, Thread * T);
	~PCB();
	static void wrapper();


	//signals
	HandlerList *handlers[MAX_SIGNAL_NUM];
	SignalList *signals;
	int blockedSignals[MAX_SIGNAL_NUM];

	//getters and setters
	ID getPid();
	Time getTime();
	unsigned getSp();
	void setSp(unsigned sp);
	unsigned getSs();
	void setSs(unsigned ss);
	unsigned getBp();
	void setBp(unsigned bp);
	int getWaitingTime();
	void setWaitingTime(int t);
	Thread* getThread();
	int isInfinite();

	//signal processor
	void  handleSignals();

	//signalHandlers
	static void SignalHandler0();
private:
	friend class Thread;
	unsigned ss_;
	unsigned sp_;
	unsigned bp_;
	unsigned * stack_;
	Thread * myThread_;
	StackSize size_;
	Time timeSlice_; //max running time before context switch, 0 is infinite
	int waitingTime_; //max waiting time on semaphore, -1 is infinite
	static ID spid;
	ID pid_;
};





#endif /* PCB_H_ */
