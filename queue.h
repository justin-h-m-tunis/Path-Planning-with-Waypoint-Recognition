#include"node.h"
template<class U> class queue {
	public:
	node<U>* start;
	node<U>* end;
	bool isEmpty() {
		return (start == NULL);
	}
	void push(U d) {
		node<U>* n = new node<U>(d);
		if (!isEmpty())
			end->next = n;
		end = n;
		if (isEmpty())
			start = end;
	}
	U pop(){
		node<U>* n = start;
		start = start->next;
		return n->data;
	}
	queue<U>() {
		start = NULL;
		end = NULL;
	}
};