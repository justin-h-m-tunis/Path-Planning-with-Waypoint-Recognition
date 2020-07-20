#include"map.h"

#define COST 1

#define PI 3.14159265
struct point_d {
	double x;
	double y;
	point_d() {
		x = 0.0;
		y = 0.0;
	}
	point_d(double X, double Y) {
		x = X;
		y = Y;
	}
	point_d operator+(point_d p) {
		return point_d(x + p.x, y + p.y);
	}
};

bool map::canReach(element* a, element* b) {
	double dist = sqrt((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
	double slopeY = (double)(b->y - a->y) / dist;
	double slopeX = (double)(b->x - a->x) / dist;
	point_d p((double)a->x, (double)a->y);
	for (int s = 0;s<dist;s++) {
		p = p + point_d(slopeX, slopeY);
		if (plot[(int)p.x][(int)p.y].obstructed) return false;
	}
	return true;
}

array<point> map::findPath(int x1, int y1, int x2, int y2, double W, bool useWP)//8 connected
{
	heap<element*> openList(height*width,&element::compareElement);
	//stack<point> path;
	array<point> path;
	int x = x1;
	int y = y1;
	plot[x1][y1].g = 0;
	do {
		plot[x][y].closed = true;
		if (plot[x2][y2].closed) break;
		for (int i = -1;i <= 1;i++) {
			for (int j = -1; j <= 1;j++) {
				if (0 <= x + i&&x + i < width && 0 <= y + j&&y + j < height
					&& !plot[x+i][y+j].obstructed && !plot[x+i][y+j].closed) {
					if (plot[x + i][y + j].g != INFINITY) {
						if (plot[x + i][y + j].g > plot[x][y].g + COST*sqrt(i*i + j*j)) {
							plot[x + i][y + j].g = plot[x][y].g + COST*sqrt(i*i + j*j);
							plot[x + i][y + j].h = sqrt((x + i - x2)*(x + i - x2) + (y + j - y2)*(y + j - y2));
							plot[x + i][y + j].f = plot[x + i][y + j].g + plot[x + i][y + j].h*W;
							openList.update(&plot[x + i][y + j]);
						}
					}
					else {
						plot[x + i][y + j].g = plot[x][y].g + COST*sqrt(i*i + j*j);
						plot[x + i][y + j].h = sqrt((x + i - x2)*(x + i - x2) + (y + j - y2)*(y + j - y2));
						plot[x + i][y + j].f = plot[x + i][y + j].g + plot[x + i][y + j].h*W;
						openList.insert(&plot[x + i][y + j]);
					}
				}
			}
		}
		if (useWP) {
			for (int w = 0; w < wayPoints->size;w++) {
				element* e = wayPoints->getArrayData(w)->e;
				if (plot[x][y].WPAcess->getArrayData(w)) {
					if (e->g != INFINITY) {
						if (e->g > plot[x][y].g + COST*sqrt((x - e->x)*(x - e->x) + (y - e->y)*(y - e->y))) {
							e->g = plot[x][y].g + COST*sqrt((x - e->x)*(x - e->x) + (y - e->y)*(y - e->y));
							e->h = sqrt((e->x - x2)*(e->x - x2) + (e->y - y2)*(e->y - y2));
							e->f = e->g + e->h * W;
							wayPoints->getArrayData(w)->xFrom = x;
							wayPoints->getArrayData(w)->yFrom = y;
							openList.update(e);
						}
					}
					else {
						e->g = plot[x][y].g + COST*sqrt((x - e->x)*(x - e->x) + (y - e->y)*(y - e->y));
						e->h = sqrt((e->x - x2)*(e->x - x2) + (e->y - y2)*(e->y - y2));
						e->f = e->g + e->h * W;
						wayPoints->getArrayData(w)->xFrom = x;
						wayPoints->getArrayData(w)->yFrom = y;
						openList.insert(e);
					}
				}
			}
		}
		if (!openList.isEmpty()) {
			element* temp = openList.pop();
			x = temp->x;
			y = temp->y;
		}
	} while (!openList.isEmpty());
	if (plot[x2][y2].closed) {
		while (x != x1 || y != y1) {
			int xNext = 0;
			int yNext = 0;
			double gMin = INFINITY;
			for (int i = -1;i <= 1;i++) {
				for (int j = -1; j <= 1;j++) {
					if (0 <= x + i&&x + i < width && 0 <= y + j&&y + j < height &&!plot[x + i][y + j].obstructed) {
						if (plot[x + i][y + j].g < gMin) {
							gMin = plot[x + i][y + j].g;
							xNext = x + i;
							yNext = y + j;
						}
					}
				}
			}
			if (useWP) {
				for (int w = 0; w < wayPoints->size;w++) {
					if (wayPoints->getArrayData(w)->e->x == x && wayPoints->getArrayData(w)->e->y) {
						if (plot[wayPoints->getArrayData(w)->xFrom][wayPoints->getArrayData(w)->yFrom].g < gMin) {
							gMin = wayPoints->getArrayData(w)->e->g;
							xNext = wayPoints->getArrayData(w)->xFrom;
							yNext = wayPoints->getArrayData(w)->yFrom;
						}
					}
				}
			}
			path.arrayInsert(point(x, y));
			x = xNext;
			y = yNext;
			//printf("([%d,%d])", x, y);
		}
		path.arrayInsert(point(x1, y1));
	}
	else
		printf("failed to find path!");
	//clear elements
	return path;
}

void map::clearMap() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			plot[i][j].g = INFINITY;
			plot[i][j].h = 0;
			plot[i][j].f = INFINITY;
			plot[i][j].closed = false;
		}
	}
	for (int w = 0; w < wayPoints->size;w++) {
		wayPoints->getArrayData(w)->e->g = INFINITY;
		wayPoints->getArrayData(w)->e->h = 0;
		wayPoints->getArrayData(w)->e->f = INFINITY;
		wayPoints->getArrayData(w)->e->closed = false;
		wayPoints->getArrayData(w)->xFrom = 0;
		wayPoints->getArrayData(w)->xFrom = 0;
	}
}

void map::drawMap(sf::RenderWindow* w){
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(10, 10));
	rect.setPosition(50, 50);

	sf::CircleShape circ;
	circ.setOrigin(4, 4);
	circ.setRadius(4);
	circ.setOutlineColor(sf::Color::Yellow);
	circ.setFillColor(sf::Color(0,0,0,0));
	circ.setOutlineThickness(2);

	/*for (int i = 0; i < width;i++) {
		for (int j = 0; j < height;j++) {
			if (plot[i][j].obstructed) {
				rect.setFillColor(sf::Color::Red);
				rect.setOutlineThickness(0);
			}
			else {
				rect.setFillColor(sf::Color::White);
				//rect.setFillColor(sf::Color(255-plot[i][j].g*.5,255- plot[i][j].g*.5,255- plot[i][j].g*.5,255));
				rect.setOutlineColor(sf::Color::Black);
				rect.setOutlineThickness(1);
			}
			rect.setPosition(50 + 2/SCALE * i, 50 + 2/SCALE * j);
			w->draw(rect);
		}
	}*/
	sf::Image img;
	img.loadFromFile("map1.png");
	for (int i = 0; i < width;i++) {
		for (int j = 0; j < height;j++) {
			if (!plot[i][j].obstructed) {
				img.setPixel(i, j, sf::Color(255, 255 * (1 - heatMap[i][j]), 255, 255));
			}
		}
		//printf("\n");
	}
	sf::Texture texture;
	texture.loadFromImage(img);
	sf::Sprite map;
	map.setTexture(texture);
	map.setPosition(50, 50);
	map.setScale(1000.0/(float)width,1000.0/(float)height);
	w->draw(map);
	for (int p = 0; p < wayPoints->size; p++) {
		circ.setPosition(50 + wayPoints->getArrayData(p)->e->x/(width/1000), 50 + wayPoints->getArrayData(p)->e->y/(height/1000));
		w->draw(circ);
	}
}

void map::updateHM(array<point>* path) {
	const int sampleLength = 10;
	const int diminishingFactor = 25;
	const int spacing = 50;
	for (int i = sampleLength; i < path->size - sampleLength-1;i++) {
		point left = path->getArrayData(i-sampleLength);
		point middle = path->getArrayData(i);
		point right = path->getArrayData(i + sampleLength);
		double theta_1 = atan((double)(middle.y - left.y) / (double)(middle.x - left.x));
		double theta_2 = atan((double)(middle.y - right.y) / (double)(middle.x - right.x));
		double theta_3 = abs(abs(theta_1) - abs(theta_2));
		bool near = false;
		for (int w = 0; w < wayPoints->size; w++) {
			if (abs(wayPoints->getArrayData(w)->e->x - middle.x) < spacing && abs(wayPoints->getArrayData(w)->e->y - middle.y) < spacing)
				near = true;
		}
		if (!near) {
			heatMap[middle.x][middle.y] += theta_3 / PI / diminishingFactor;
			for (int d1 = -100; d1 <= 100;d1++) {
				for (int d2 = -100; d2 <= 100;d2++) {
					if (middle.x + d1 >= 0 && middle.x + d1 < width && middle.y + d2 >= 0 && middle.y + d2 < height&&!(d1 == 0 && d2 == 0) && !plot[middle.x + d1][middle.y + d2].obstructed) {
						heatMap[middle.x + d1][middle.y + d2] += theta_3*(100 * sqrt(2) - sqrt(d1*d1 + d2*d2)) / 100 * sqrt(2) / diminishingFactor / PI;
					}
				}
			}
		}
	}
	normalizeHM(.99,50);
}

void map::normalizeHM(double threshold, int spacing) {
	double max = 0.0;
	int xMax = 0;
	int yMax = 0;
	for (int i = 0; i < width;i++) {
		for (int j = 0; j < height;j++) {
			if (heatMap[i][j] > max) {
				max = heatMap[i][j];
				xMax = i;
				yMax = j;
			}
		}
	}
	if (max > 1.0) {
		for (int i = 0; i < width;i++) {
			for (int j = 0; j < height;j++) {
				heatMap[i][j] /= (max);
				bool duplicate = false;
				for (int w = 0; w < wayPoints->size; w++) {
					if (abs(wayPoints->getArrayData(w)->e->x - xMax) < spacing && abs(wayPoints->getArrayData(w)->e->y - yMax) < spacing)
						duplicate = true;
				}
				if (!duplicate) {
					wayPoints->arrayInsert(new wayPoint(&plot[xMax][yMax], 0, 0));
					for (int X = 0; X < width;X++) {
						for (int Y = 0; Y < height; Y++) {
							plot[X][Y].WPAcess->arrayInsert(canReach(&plot[X][Y], wayPoints->getArrayData(wayPoints->size-1)->e));
						}
					}
					for (int a = -1*spacing; a < spacing; a++) {
						for (int b = -1*spacing; b < spacing; b++) {
							if (a + i < width&&a + i >= 0 && b + j < height&&b + j >= 0) {
								heatMap[i + a][j + b] = 0;
							}
						}
					}
				}
			}
		}
	}
	printf("\n# of WP: %d\n", wayPoints->size);
}


