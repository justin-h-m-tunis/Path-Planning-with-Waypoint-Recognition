#pragma once
template<class U> class heap {
	int capacity;
	int size;
	U* data;
public:
	int(*compare)(U, U);//-1 for higher prioriy,1 for lower priority, 0 for same;
	heap<U>(int cap, int(*com)(U, U)) {
		capacity = cap;
		compare = com;
		data = new U[capacity];
		size = 0;
	}
	bool isEmpty() {
		return size == 0;
	}
	void siftUp(int i) {
		while (i!=1&&compare(data[i], data[i / 2]) == -1) {
			U temp = data[i];
			data[i] = data[i / 2];
			data[i / 2] = temp;
			i = i / 2;
		}
	}
	void siftDown(int i) {
		while ((2*i<=size&&compare(data[i],data[2*i])==1)||(2*i+1<=size&&compare(data[i],data[2*i+1])==1)) {
			if (2 * i + 1 > size||compare(data[2*i],data[2*i+1])==-1) {
				U temp = data[i];
				data[i] = data[2*i];
				data[2*i] = temp;
				i = i*2;
			}
			else {
				U temp = data[i];
				data[i] = data[2 * i+1];
				data[2 * i+1] = temp;
				i = 2*i+1;
			}
		}
	}
	void insert(U cell) {
		size++;
		data[size] = cell;
		siftUp(size);
	}
	U pop() {
		U temp = data[1];
		data[1] = data[size];
		size--;
		if(!isEmpty())
			siftDown(1);
		return temp;
	}
	bool update(U target) {
		for (int i = 0; i <= size; i++) {
			if (data[i] == target) {
				siftUp(i);
				return true;
			}
		}
		return false;
	}
};