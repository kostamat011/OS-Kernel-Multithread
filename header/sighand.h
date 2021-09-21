/*
 * sighand.h
 *
 *  Created on: May 10, 2020
 *      Author: OS1
 */

#ifndef SIGHAND_H_
#define SIGHAND_H_

typedef void (*SignalHandler)();
typedef unsigned SignalId;

struct SignalHandlerNode {
	SignalHandler handler;
	SignalId signalNum;
	SignalHandlerNode(){handler = 0; signalNum=-1;}
	SignalHandlerNode(SignalHandler sh, SignalId id){handler = sh; signalNum=id;}
};


#endif /* SIGHAND_H_ */
