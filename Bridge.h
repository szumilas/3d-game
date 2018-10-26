#pragma once

#include "MapObject.h"

class Bridge : public MapObject
{
public:

	Bridge(MapObject& mapObject) : MapObject(mapObject) {};

	void display();

};