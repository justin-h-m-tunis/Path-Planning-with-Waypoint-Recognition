#include <SFML/Graphics.hpp>
#include"map.h"

#define COST 1/1
#define SCALE 1

sf::Texture loadTexture(std::string filename) {
	sf::Texture texture;
	texture.loadFromFile(filename);
	return texture;
}

sf::Sprite loadSprite(sf::Texture* texture) {
	sf::Sprite sprite;
	sprite.setTexture(*texture);
	return sprite;
}

sf::Sprite loadSprite(sf::Texture* texture, sf::IntRect rect) {
	sf::Sprite sprite;
	sprite.setTexture(*texture);
	sprite.setTextureRect(rect);
	return sprite;
}

sf::Image* obstructionMap = new sf::Image();

bool isObstructed(point p) {
	sf::Color c = obstructionMap->getPixel(p.x, p.y);
	if(c.r == 0 && c.g == 0 && c.b == 0)
		return true;
	else return false;
}

void drawPath(sf::RenderWindow* w, array<point> a, bool print) {
	int x1, y1, x2, y2;
	x1 = a.getArrayData(a.size - 1).x;
	y1 = a.getArrayData(a.size - 1).y;
	for (int i = a.size - 2; i >= 0;i--) {
		x2 = x1;
		y2 = y1;
		x1 = a.getArrayData(i).x;
		y1 = a.getArrayData(i).y;
		//printf("(%d,%d)\n", x1, y1);
		sf::Vertex line[] = { sf::Vertex(sf::Vector2f(50 + x1/1, 50 + y1/1)),sf::Vertex(sf::Vector2f(50 + x2/1, 50 + y2/1)) };
		line[0].color = sf::Color::Blue;
		line[1].color = sf::Color::Blue;
		w->draw(line, 2, sf::Lines);
	}
}
int compareInt(int a, int b) {
	if (a < b)return -1;
	else if (a == b)return 0;
	else return 1;
}

void updateData(double (*dA)[26], double (*dP)[26], double v, int location) {
	if (location == 0 && (*dA)[0]!=INFINITY) {
		(*dA)[location] = ((*dA)[25] * 26 - (*dP)[0] + v) / 26.0;
		//printf("a->%f,%d\n", (*dA)[location], location);
	}
	else if (location == 0 && (*dA)[0] == INFINITY) {
		(*dA)[location] = v;
		//printf("b->%f,%d\n", (*dA)[location], location);
	}
	else if ((*dA)[location] == INFINITY) {
		(*dA)[location] = ((*dA)[location - 1]*location + v)/(location+1);
		//printf("c->%f,%d\n", (*dA)[location],location);
	}
	else {
		(*dA)[location] = ((*dA)[location - 1] * 26.0 - (*dP)[location] + v) / 26.0;
		//printf("d->%f,%d\n", (*dA)[location], location);
	}
	(*dP)[location] = v;
	
}

void displayData(double(*dA)[26], double(*dP)[26],int location, sf::RenderWindow* g) {
	sf::Vertex xAxis[] = { sf::Vertex(sf::Vector2f(20,150)),sf::Vertex(sf::Vector2f(280,150 )) };
	sf::Vertex yAxis[] = { sf::Vertex(sf::Vector2f(20, 20)),sf::Vertex(sf::Vector2f(20, 280)) };
	xAxis[0].color = sf::Color::Cyan;
	xAxis[1].color = sf::Color::Cyan;
	yAxis[0].color = sf::Color::Cyan;
	yAxis[1].color = sf::Color::Cyan;

	sf::CircleShape p;
	p.setOrigin(3, 3);
	p.setRadius(3);
	p.setOutlineColor(sf::Color::Magenta);
	p.setFillColor(sf::Color(0, 0, 0, 0));
	p.setOutlineThickness(2);

	sf::CircleShape a;
	a.setOrigin(3, 3);
	a.setRadius(3);
	a.setOutlineColor(sf::Color::Yellow);
	a.setFillColor(sf::Color(0, 0, 0, 0));
	a.setOutlineThickness(2);

	g->draw(xAxis,2,sf::Lines);
	g->draw(yAxis,2,sf::Lines);

	for (int i = 0; i < 26;i++) {
		if ((*dP)[i] == INFINITY) break;
		int x;
		if (i > location) {
			x = i - location;
		}
		else {
			x = 26 - location + i;
		}
		p.setPosition(10 * x, -(*dP)[i] * 130+150);
		a.setPosition(10 * x, -(*dA)[i] * 130+150);

		g->draw(p);
		g->draw(a);

		if (i == 0) continue;
		
		sf::Vertex pLine[] = { sf::Vertex(sf::Vector2f(10 * x, -(*dP)[i] * 130+150)),sf::Vertex(sf::Vector2f(10 * (x-1), -(*dP)[i-1] * 130+150)) };
		sf::Vertex aLine[] = { sf::Vertex(sf::Vector2f(10 * x, -(*dA)[i] * 130+150)),sf::Vertex(sf::Vector2f(10 * (x-1), -(*dA)[i - 1] * 130+150)) };
		pLine[0].color = sf::Color::Magenta;
		pLine[1].color = sf::Color::Magenta;
		aLine[0].color = sf::Color::Yellow;
		aLine[1].color = sf::Color::Yellow;

		g->draw(pLine, 2, sf::Lines);
		g->draw(aLine, 2, sf::Lines);
		
	}
}

int main()
{

	sf::RenderWindow window(sf::VideoMode(1100, 1100), "Pathfinding");
	sf::RenderWindow graph(sf::VideoMode(300, 300), "Data");
	double dataPoints[26];
	double dataAvg[26];
	for (int i = 0; i < 26;i++) {
		dataAvg[i] = INFINITY;
		dataPoints[i] = INFINITY;
	}
	obstructionMap->loadFromFile("map1.png");
	sf::Clock runTime;

	map* course = new map(&isObstructed);
	/*wayPoint* wp1 = new wayPoint(&course->plot[325][282],0,0);//new element(INFINITY, 0, INFINITY, 100, 100, 0);
	wayPoint* wp2 = new wayPoint(&course->plot[577][285],0,0);//new element(INFINITY, 0, INFINITY, 50, 150, 0);
	course->wayPoints->arrayInsert(wp1);
	course->wayPoints->arrayInsert(wp2);*/

	srand(103357);
	int total = 0;
	double sum = 0;
	sf::Time t1;
	sf::Time t2;
	while(window.isOpen()&&graph.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		while (graph.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				graph.close();
		}

		window.clear();
		graph.clear();
		course->clearMap();
		int x1, y1, x2, y2;
		do {
			x1 = rand() % course->width;
			x2 = rand() % course->width;
			y1 = rand() % course->height;
			y2 = rand() % course->height;
		} while (course->plot[x1][y1].obstructed || course->plot[x2][y2].obstructed);
		runTime.restart();
		array<point> path = course->findPath(x1, y1, x2, y2, 2,true);
		t1 = runTime.getElapsedTime();
		if (path.size == 0) continue;
		printf("WWA* runtime: %d\n", t1.asMilliseconds());

		course->clearMap();
		runTime.restart();
		course->findPath(x1, y1, x2, y2, 2, false);
		t2 = runTime.getElapsedTime();
		printf("weighted A* runtime: %d\n", t2.asMilliseconds());

		if (abs((double)(t1 - t2).asMilliseconds() / (double)t2.asMilliseconds()) < 1.0) {
			updateData(&dataAvg, &dataPoints, (double)(t1 - t2).asMilliseconds() / (double)t2.asMilliseconds(), total % 26);
			displayData(&dataAvg, &dataPoints, total % 26, &graph);
			total++;
			sum += (double)(t1 - t2).asMilliseconds();
		}
		printf("Time lost: %f\n",sum);
		if (course->wayPoints->size < 15) {
			course->updateHM(&path);
		}
		//else {
			course->drawMap(&window);
			drawPath(&window, path, true);
		//}
		window.display();
		graph.display();
	}

	return 0;
}