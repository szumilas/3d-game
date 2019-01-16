#pragma once

#include "MapObject.h"

class Railway : public MapObject
{
public:

	Railway(MapObject& mapObject) : MapObject(mapObject) { _min_height = 0.05f; };

	void calculateFinalGeometry(TextureManager* textureManager);
	void calculateXYfromRef(const std::map<long long, node> &nodes);

private:

	float gauge = 1.435f;

	std::vector<Point> finalLeftRail;
	std::vector<Point> finalRightRail;

};