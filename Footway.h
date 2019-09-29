#pragma once

#include "MapObject.h"

class Footway : public MapObject
{
public:

	Footway(MapObject& mapObject) : MapObject(mapObject)
	{
		_min_height = -0.05f;
		_color.red = 1.0f;
		_color.green = 1.0f;
		_color.blue = 1.0f;
		textureName = Textures::paving;
	};

	virtual void calculateFinalGeometry();
	void calculateXYfromRef(const std::map<long long, node> &nodes);

protected:

	float gauge = 2.0f;
	Textures textureName;

	std::vector<Point> finalLeftRail;
	std::vector<Point> finalRightRail;

private:

};