#pragma once

#include "MapObject.h"

class HighlightedObject : public MapObject
{
public:

	HighlightedObject(MapObject& mapObject) : MapObject(mapObject) {};

	void display();

};