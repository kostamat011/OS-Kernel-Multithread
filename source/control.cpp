/*
 * control.cpp
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */
#include "control.h"
#include "inter.h"
#include <dos.h>
#include <stdio.h>

volatile int lock = 0;

unsigned oldTimerSEG = 0;
unsigned oldTimerOFF = 0;

//MOVE NEW TIMER ROUTINE TO 8h AND MOVE OLD TIMER ROUTINE TO 60h
void inicIVTP(){
#ifndef BCC_BLOCK_IGNORE
    asm {
        cli
        push es
        push ax
        mov ax,0
        mov es,ax
        mov ax, word ptr es:0022h

        mov word ptr oldTimerSEG, ax
        mov ax, word ptr es:0020h
        mov word ptr oldTimerOFF, ax
        mov word ptr es:0022h, seg timer

        mov word ptr es:0020h, offset timer
        mov ax, oldTimerSEG
        mov word ptr es:0182h, ax

        mov ax, oldTimerOFF
        mov word ptr es:0180h, ax
        pop ax
        pop es
        sti
    }
#endif
}


//RESTORE OLD TIMER ROUTINE TO 8h
void restoreIVTP(){
#ifndef BCC_BLOCK_IGNORE
    asm {
        cli
        push es
        push ax
        mov ax,0
        mov es,ax
        mov ax, word ptr oldTimerSEG
        mov word ptr es:0022h, ax
        mov ax, word ptr oldTimerOFF
        mov word ptr es:0020h, ax
        pop ax
        pop es
        sti
    }
#endif
}


