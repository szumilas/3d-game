#pragma once

#include "MapObject.h"

class StreetLamp : public MapObject
{
public:

	StreetLamp(long long ref);

	void calculateXYfromRef(const std::map<long long, node> &nodes);

private:
	float size;

};