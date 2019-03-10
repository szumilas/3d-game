#pragma once

#include "Building.h"

class Sedesowiec : public Building
{

public:

	Sedesowiec(MapObject& mapObject);

private:

	void generateWalls();

};