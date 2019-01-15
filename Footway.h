#pragma once

#include "MapObject.h"

class Footway : public MapObject
{
public:

	Footway(MapObject& mapObject) : MapObject(mapObject) {
		_min_height = -0.05f;
	};

	void calculateFinalGeometry(TextureManager* textureManager);
	void calculateXYfromRef(const std::map<long long, node> &nodes);

private:

	float gauge = 2.0f;
	unsigned int idTexture;

	std::vector<Point> finalLeftRail;
	std::vector<Point> finalRightRail;

};