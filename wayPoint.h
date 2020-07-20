#pragma once
#include "element.h"

class wayPoint {
public:
	element* e;
	int xFrom;
	int yFrom;
	wayPoint() {};
	wayPoint(element* E, int x, int y) {
		e = E;
		xFrom = x;
		yFrom = y;
	}
};