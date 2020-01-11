#pragma once

#include "Building.h"

class GrunwaldzkiCenter : public Building
{

public:

	GrunwaldzkiCenter(MapObject& mapObject);
	void calculateFinalGeometry();
	void display() { MapObject::display(); }

private:

	void generateWalls();

};