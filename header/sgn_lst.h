/*
 * sgn_lst.h
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#ifndef SGN_LST_H_
#define SGN_LST_H_

struct SignalNode{
	SignalId signal;
	SignalNode * next;
	SignalNode(SignalId id){signal=id; next=0;}
};

class SignalList{
public:
	SignalNode * head;
	SignalList(){
		head=0;
	}
	~SignalList(){
		lockSW
		SignalNode *tmp;
		while(head){
			tmp=head;
			head=head->next;
			delete tmp;
		}
		unlockSW
	}
	void insertSignal(SignalId id){
		SignalNode *sn = new SignalNode(id);
		if(!head)
			head=sn;
		else{
			SignalNode *tmp = head;
			while(tmp->next){
					tmp=tmp->next;
			}
			tmp->next=sn;
		}
	}
	SignalList * copy(){
		SignalList *clone = new SignalList();
		SignalNode *tmp = head;
		while(tmp){
			clone->insertSignal(tmp->signal);
			tmp=tmp->next;
		}
		return clone;
	}
	void removeSignal(SignalId id){
		SignalNode *tmp = head;
		SignalNode *prev = 0;
		while(tmp && tmp->signal!=id){
			tmp=tmp->next;
		}
		if(!tmp) //not found
			return;

		if(!prev)
			head=tmp->next;
		else
			prev->next = tmp->next;

		delete tmp;
	}
};




#endif /* SGN_LST_H_ */
