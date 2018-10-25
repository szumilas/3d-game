#pragma once

#include "MapObject.h"

class Building : public MapObject
{
public:

	Building(MapObject& mapObject) : MapObject(mapObject) {};

	void display();

};