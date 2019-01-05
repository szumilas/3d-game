#pragma once

#include "MapObject.h"

class Building : public MapObject
{
public:

	Building(MapObject& mapObject);

	void display();

private:

	void calculateXYfromRef(const std::map<long long, node> &nodes);
	void generateWalls();

public:


private:

	struct Wall
	{
		Point p1;
		Point p2;
		Color color;
	};

	std::vector<Wall> walls;

};