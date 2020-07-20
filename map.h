#pragma once
#include "wayPoint.h"
#include<SFML\Graphics.hpp>
#include <queue\queue.h>
#include <queue\stack.h>
#include "poly.h"
#include <queue\heap.h>

#define SCALE 1

#define WIDTH 1000
#define HEIGHT 1000

class map {
public:
	int height;
	int width;
	element plot[WIDTH *SCALE][HEIGHT * SCALE];
	double heatMap[WIDTH *SCALE][HEIGHT * SCALE];

	array<wayPoint*>* wayPoints;

	array<point> findPath(int x1, int y1, int x2, int y2,double W,bool useWP);
	void clearMap();
	void drawMap(sf::RenderWindow* w);
	bool canReach(element* a, element* b);
	void updateHM(array<point>* path);
	void normalizeHM(double threshold, int spacing);

	map() {
		height = HEIGHT*SCALE;
		width = WIDTH*SCALE;
		wayPoints = new array<wayPoint*>();
		for (int i = 0; i < width;i++) {
			for (int j = 0; j < height;j++) {
				plot[i][j] = element(INFINITY, 0, INFINITY, i, j, 0);
				heatMap[i][j] = 0 / (width*height);
			}
		}
		normalizeHM(100,width);
	}
	map(bool (*mapFn)(point)) {
		height = HEIGHT * SCALE;
		width = WIDTH * SCALE;
		wayPoints = new array<wayPoint*>();
		for (int i = 0; i < width;i++) {
			for (int j = 0; j < height;j++) {
				plot[i][j] = element(INFINITY, 0, INFINITY, i, j, (*mapFn)(point(i,j)));
				if (!plot[i][j].obstructed)
					heatMap[i][j] = 0;
				else
					heatMap[i][j] = 0;
			}
		}
		normalizeHM(100,width);
	}
};
