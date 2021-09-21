/*
 * control.h
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */

#ifndef CONTROL_H_
#define CONTROL_H_

#define lockSW ++lock;
#define unlockSW --lock;
#define lockINTR  {asm{ pushf; cli;}}
#define unlockINTR  {asm{ popf }}


typedef void interrupt (*pInterrupt)(...);

volatile extern int updateLock; //in ksem.cpp
volatile extern int lock; //in control.cpp
volatile extern int counter; //in inter.cpp
volatile extern int dispatchExplicit; //in thread.cpp


void inicIVTP();
void restoreIVTP();


#endif /* CONTROL_H_ */
