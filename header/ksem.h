/*
 * kernSem.h
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */

#ifndef KSEM_H_
#define KSEM_H_

class PCB;

typedef unsigned int Time;
typedef int ID;

template<class T>
class List;

class KernelSem{
public:

	static List<KernelSem> * allKernelSem;
	static void updateAllKernelSem();
	static int ssid;

	KernelSem(int val);
	~KernelSem();

	int signal(int n=0);
	int wait(Time maxTimeToWait);
	int getVal();
	ID getSid();

protected:
	void updateBlockedList();
	friend class PCB;

private:
	int val_;
	int sid_;
	List<PCB> * blockedList;
};




#endif /* KSEM_H_ */
