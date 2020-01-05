#pragma once

#include "Building.h"

class PasazGrunwaldzki : public Building
{

public:

	PasazGrunwaldzki(MapObject& mapObject);
	void calculateFinalGeometry();
	void display() { Object3D::display(); }

private:

	void generateWalls();

};