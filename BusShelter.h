#pragma once

#include "Building.h"

class BusShelter : public Building
{
public:

	BusShelter(MapObject& mapObject);
	void calculateFinalGeometry();

private:

	void generateWalls();

};