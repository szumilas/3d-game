#pragma once

#include "Building.h"

class Sedesowiec : public Building
{

public:

	Sedesowiec(MapObject& mapObject);
	void display() { MapObject::display(); }

private:

	void generateWalls();

};