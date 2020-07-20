#pragma once
#pragma once

struct point {
	int x, y;
	point() {
		x = 0;
		y = 0;
	}
	point(int X, int Y) {
		x = X;
		y = Y;
	}
	point operator+(point p) {
		return point(x + p.x, y + p.y);
	}
};
struct edge {
	point a, b;
	double slope;
	edge() {
		a = point();
		b = point();
		if ((a.x - b.x) != 0)
			slope = (double)(a.y - b.y) / (double)(a.x - b.x);
		else
			slope = INFINITY;
	}
	edge(point A, point B) {
		a = A;
		b = B;
		if ((a.x - b.x) != 0)
			slope = (double)(a.y - b.y) / (double)(a.x - b.x);
		else
			slope = INFINITY;

		if (slope > .5&&slope < 1.5)
			slope = 1;
		if (slope < -.5&&slope > -1.5)
			slope = -1;
		if (slope < .5&&slope > -.5)
			slope = 0;
		if (slope > 10)
			slope = INFINITY;
	}

	bool operator>(point p) {//to the right
							 //x value of edge at height y
		double X_y = (double)a.x + (double)(p.y-a.y) / slope;
		return ((double)p.x >= X_y);
	}
	bool operator<(point p) {//to the left
		double X_y = (double)a.x + (double)(p.y-a.y) / slope;
		return ((double)p.x <= X_y);
	}
	bool operator^(point p) {//above
		double Y_x = (double)a.y + (double)(p.x-a.x) * slope;
		return ((double)p.x >= Y_x);
	}
	bool operator-(point p) {//below
		double Y_x = (double)a.y + (double)(p.x-a.x) * slope;
		return ((double)p.x <= Y_x);
	}
};
class poly {
	point vertex[4];//edges are on consecutive numbers (0-1, 1-2, 2-0)
	edge edges[4];
public:
	poly(point left, point top, point right, point down) {
		vertex[0] = left;
		vertex[1] = top;
		vertex[2] = right;
		vertex[3] = down;
		edges[0] = edge(vertex[0], vertex[1]);
		edges[1] = edge(vertex[1], vertex[2]);
		edges[2] = edge(vertex[2], vertex[3]);
		edges[3] = edge(vertex[3], vertex[0]);
	}
	bool isIn(point p) {
		return (edges[0] > p) && (edges[1] < p) && (edges[2] < p) && (edges[3] > p);
	}
};

