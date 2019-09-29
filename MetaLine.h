#pragma once

#include "Crossing.h"

class MetaLine : public Crossing
{
public:

	MetaLine(MapObject& mapObject);

	std::pair<Point, Point> getMetaLinePoints();

private:

	void adjustTextures();

public:


private:

};