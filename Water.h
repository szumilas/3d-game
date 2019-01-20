#pragma once

#include "MapObject.h"

class Water : public MapObject
{
public:

	Water(MapObject& mapObject) : MapObject(mapObject) {};

	void calculateFinalGeometry(TextureManager* textureManager);

private:


public:


private:

};