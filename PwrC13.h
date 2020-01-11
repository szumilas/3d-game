#pragma once

#include "Building.h"

class PwrC13 : public Building
{

public:

	PwrC13(MapObject& mapObject);
	void display() { MapObject::display(); }

private:

	void generateWalls();

};