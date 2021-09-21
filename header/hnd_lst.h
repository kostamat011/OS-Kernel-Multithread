/*
 * hnd_lst.h
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */
#ifndef HND_LST_H_
#define HND_LST_H_
typedef unsigned SignalId;
typedef void (*SignalHandler)();

struct HandlerNode{
	SignalHandler handler;
	HandlerNode * next;
	HandlerNode(SignalHandler sh){handler=sh; next=0;}
};

class HandlerList{
public:
	HandlerNode * head;
	HandlerList(){
		head=0;
	}
	~HandlerList(){
		lockSW
		HandlerNode *tmp;
		while(head){
			tmp=head;
			head=head->next;
			delete tmp;
		}
		unlockSW
	}
	void insertHandler(SignalHandler sh){
		HandlerNode *hn = new HandlerNode(sh);
		if(!head)
			head=hn;
		else{
			HandlerNode *tmp = head;
			while(tmp->next){
					tmp=tmp->next;
			}
			tmp->next=hn;
		}
	}
	HandlerList * copy(){
		HandlerList *clone = new HandlerList();
		HandlerNode *tmp = head;
		while(tmp){
			clone->insertHandler(tmp->handler);
			tmp=tmp->next;
		}
		return clone;
	}
};

#endif /* HND_LST_H_ */
