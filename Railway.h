#pragma once

#include "MapObject.h"

class Railway : public MapObject
{
public:

	Railway(MapObject& mapObject) : MapObject(mapObject) {};

	void display();
	void calculateXYfromRef(const std::map<long long, node> &nodes);

private:

	float gauge = 1.435f;

};