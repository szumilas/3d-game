#pragma once

#include "MapObject.h"

class Common : public MapObject
{
public:

	Common(MapObject& mapObject) : MapObject(mapObject) {
	
		_min_height = -0.08;

	};

	void display();

};