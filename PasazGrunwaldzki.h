#pragma once

#include "Building.h"

class PasazGrunwaldzki : public Building
{

public:

	PasazGrunwaldzki(MapObject& mapObject);
	void calculateFinalGeometry();

private:

	void generateWalls();

};