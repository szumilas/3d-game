#pragma once

#include "MapObject.h"

class Railway : public MapObject
{
public:

	Railway(MapObject& mapObject);

	void calculateFinalGeometry();
	void calculateXYfromRef(const std::map<long long, node> &nodes);

private:

	float gauge = 1.435f;

	std::vector<Point> finalLeftRail;
	std::vector<Point> finalRightRail;

};