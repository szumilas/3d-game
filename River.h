#pragma once

#include "MapObject.h"

class River : public MapObject
{
public:

	River(MapObject& mapObject) : MapObject(mapObject) {};

	void display();

};