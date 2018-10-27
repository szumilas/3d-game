#pragma once

#include "MapObject.h"

class HighlightedObject : public MapObject
{
public:

	HighlightedObject(MapObject& mapObject) : MapObject(mapObject) {
	
		_height = 50.0f;
	
	};

	void display();

private:


};