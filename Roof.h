#pragma once

#include "MapObject.h"

class Roof : public MapObject
{
public:

	Roof(MapObject& mapObject);

	void display();

private:

	void calculateXYfromRef(const std::map<long long, node> &nodes);

public:


private:

	std::vector<Point> specialPoints;
	std::vector<Point> roofEdges;
	float _roof_level;

};