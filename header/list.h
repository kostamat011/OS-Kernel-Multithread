/*
 * list.h
 *
 *  Created on: May 5, 2020
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_
template<class T>
struct Node{
	Node<T> *next;
	T * info;
	Node(T * inf){next=0;info=inf;}
	Node(){next=0;info=0;}

};

template<class T>
class List{
public:
	List();
	~List();
	void push_back(T * inf);
	Node<T> * get_head();
	Node<T> * pop_head();
	Node<T> * get_byInfo(T * inf);
	List<T> * copy();
	void remove_byInfo(T * inf);
	unsigned get_size();
	void clear();
private:
	Node<T> *head_;
	unsigned size_;

};


#endif /* LIST_H_ */

template<class T>
List<T>::List(){
	head_=0;
	size_=0;
}

template<class T>
List<T>::~List(){
	clear();
}

template<class T>
void List<T>::clear(){
	lockSW
		Node<T> * tmp;
		while(head_){
			tmp  = head_;
			head_=head_->next;
			delete tmp;
		}
	unlockSW
}


template<class T>
void List<T>::push_back(T* inf){

	Node<T> * newNode = new Node<T>(inf);
	if(!head_)
		head_=newNode;
	else{
		Node<T> * tmp = head_;
		while(tmp->next)
			tmp=tmp->next;
		tmp->next = newNode;
	}
	++size_;

}


template<class T>
Node<T> * List<T>::pop_head(){

	Node<T> * ret = head_;
	if(head_)
	{
		head_=head_->next;
	}
	--size_;

	return ret;

}

template<class T>
Node<T> * List<T>::get_head(){
	return head_;
}

template<class T>
Node<T> * List<T>::get_byInfo(T* inf){

	Node<T> * tmp = head_;
	while(tmp && tmp->info!=inf)
		tmp=tmp->next;

	return tmp;

}

template<class T>
void  List<T>::remove_byInfo(T* inf){
	Node<T> * tmp = head_;
	Node<T> * prev = 0;
	while(tmp && tmp->info!=inf)
	{
		prev=tmp;
		tmp=tmp->next;
	}
	if(!tmp)
		return;
	if(!prev)
		head_=tmp->next;
	else
		prev->next=tmp->next;
	delete tmp;
}

template<class T>
unsigned List<T>::get_size(){
	return size_;
}

template<class T>
List<T> * List<T>::copy(){
	List<T> * clone = new List<T>();
	Node<T> * tmp = this->head_;
	while(tmp){
		clone->push_back(tmp->info);
		tmp=tmp->next;
	}
	return clone;
}





