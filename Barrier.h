#pragma once

#include "MapObject.h"

class Barrier : public MapObject
{
public:

	Barrier(MapObject& mapObject) : MapObject(mapObject) {};

	void display();

};