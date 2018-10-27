#pragma once

#include "MapObject.h"

class Footway : public MapObject
{
public:

	Footway(MapObject& mapObject) : MapObject(mapObject) {
		_min_height = -0.05f;
	};

	void display();
	void calculateXYfromRef(const std::map<long long, node> &nodes);

private:

	float gauge = 2.0f;

};