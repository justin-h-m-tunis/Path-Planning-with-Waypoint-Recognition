#include <queue\array.h>
class element {
private:

public:
	double g;
	double h;
	double f;
	int x;
	int y;
	bool closed;
	bool obstructed;
	array<bool>* WPAcess;
	element() {}
	element(double G, double H, double F, int X, int Y, bool Ob)
		:g(G), h(H), f(F), x(X), y(Y), obstructed(Ob) {
		closed = 0;
		WPAcess = new array<bool>();
	}
	static int compareElement(element* e1, element* e2) {
		if (e1->f < e2->f) return -1;
		else if (e1->f == e2->f) return 0;
		else return 1;
	}
};
