#pragma once

#include "MapObject.h"

class StreetLamp : public MapObject
{
public:

	StreetLamp(long long ref);

	void display();
	void calculateXYfromRef(const std::map<long long, node> &nodes);

};