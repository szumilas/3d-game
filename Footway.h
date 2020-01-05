#pragma once

#include "MapObject.h"

class Footway : public MapObject
{
public:

	Footway();
	Footway(MapObject& mapObject);

	virtual void calculateFinalGeometry();
	void calculateXYfromRef(const std::map<long long, node> &nodes);

protected:

	float gauge = 2.0f;
	Textures textureName;

	std::vector<Point> finalLeftRail;
	std::vector<Point> finalRightRail;

private:

};