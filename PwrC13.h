#pragma once

#include "Building.h"

class PwrC13 : public Building
{

public:

	PwrC13(MapObject& mapObject);
	void display() { Object3D::display(); }

private:

	void generateWalls();

};