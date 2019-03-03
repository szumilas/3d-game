#pragma once

#include "Building.h"

class GrunwaldzkiCenter : public Building
{

public:

	GrunwaldzkiCenter(MapObject& mapObject);
	void calculateFinalGeometry();

private:

	void generateWalls();

};