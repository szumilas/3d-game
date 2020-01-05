#pragma once

#include "Building.h"

class Sedesowiec : public Building
{

public:

	Sedesowiec(MapObject& mapObject);
	void display() { Object3D::display(); }

private:

	void generateWalls();

};