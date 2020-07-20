#pragma once
template<class U> class node {
public:
	U data;
	node<U>* next;
	node(U d) {
		data = d;
		next = NULL;
	}
};