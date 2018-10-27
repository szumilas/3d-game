#pragma once

#include "MapObject.h"

class Building : public MapObject
{
public:

	Building(MapObject& mapObject) : MapObject(mapObject) {

		if (building == "roof" || building_part == "roof")
		{
			if (!_height)
			{
				_height = 6.5f;
			}
			_min_height = std::min(6.0f, _height - 0.5f);
		}
		if (!_height)
		{
			_height = 15.0f;
		}
	};

	void display();

};