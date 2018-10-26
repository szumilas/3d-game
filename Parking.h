#pragma once

#include "MapObject.h"

class Parking : public MapObject
{
public:

	Parking(MapObject& mapObject) : MapObject(mapObject) {};

	void display();

};