#pragma once

#include "Building.h"

class GrunwaldzkiCenter : public Building
{

public:

	GrunwaldzkiCenter(MapObject& mapObject);
	void calculateFinalGeometry();
	void display() { Object3D::display(); }

private:

	void generateWalls();

};