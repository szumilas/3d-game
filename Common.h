#pragma once

#include "MapObject.h"

class Common : public MapObject
{
public:

	Common(MapObject& mapObject) : MapObject(mapObject) {};

	void display();

};