#include"node.h"
template<class U> class stack{
	public:
	node<U>* start;
	bool isEmpty() {
		return (start == NULL);
	}
	void push(U d) {
		node<U>* n = new node<U>(d);
		n->next = start;
		start = n;
	}
	U pop(){
		node<U>* n = start;
		start = start->next;
		return n->data;
	}
	stack<U>() {
		start = NULL;
	}
};