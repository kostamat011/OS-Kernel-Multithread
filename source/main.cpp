
#include <stdio.h>
#include "thread.h"
#include "pcb.h"
#include "control.h"
#include "idleThr.h"
#include "list.h"

PCB * mainPCB;
IdleThread *idleT;
Thread * mainThread;


extern int userMain(int argc, char*argv[]);

int main(int argc,char* argv[]){

	PCB::initGlobalSignalFlags();
	mainPCB = new PCB(0,0,0);
	mainPCB->status=RUNNING;
	mainThread = new Thread(mainPCB);
	mainThread->start();
	PCB::running=mainPCB;

	idleT = new IdleThread();
	idleT->start();

	inicIVTP();
	argv[1]="120";
	argv[2]="19";
	argv[3]="0";
	int ret = userMain(4,argv);
	restoreIVTP();

	idleT->stop();
	delete idleT;
	delete mainThread;
	return ret;
}
