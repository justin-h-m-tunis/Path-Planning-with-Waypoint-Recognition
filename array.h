template<class U> class array{
		U* data;
		int capacity;
	public:
		int size;
		array(int start) {
			size = 0;
			capacity = start;
			data = new U[capacity];
		}
		array() {
			size = 0;
			capacity = 1;
			data = new U[capacity];
		}
		void doubleArray() {
			capacity *= 2;
			U* temp = data;
			data = new U[capacity];
			for (int i = 0; i < size; i++) {
				data[i] = temp[i];
			}
		}
		void arrayInsert(U d) {
			data[size] = d;
			size++;
			if (size == capacity) {
				doubleArray();
			}
		}

		U getArrayData(int i) {
			return data[i];
		}
};