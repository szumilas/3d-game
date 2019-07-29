#pragma once

#include "MapObject.h"

class Barrier : public MapObject
{
public:

	Barrier(MapObject& mapObject);

	void calculateFinalGeometry();
	
private:
	Textures textureName;

};