#pragma once

#include "MapObject.h"

class River : public MapObject
{
public:

	River(MapObject& mapObject) : MapObject(mapObject) {
		_min_height = -3.0f;
	};

	void calculateFinalGeometry();
	void calculateXYfromRef(const std::map<long long, node> &nodes);

private:

	float gauge = 50.0f;
	unsigned int idTexture;

	std::vector<Point> finalLeftRail;
	std::vector<Point> finalRightRail;

};