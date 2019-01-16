#pragma once

#include "MapObject.h"

class GreenArea : public MapObject
{
public:

	GreenArea(MapObject& mapObject) : MapObject(mapObject) {};

	void calculateFinalGeometry(TextureManager* textureManager);

private:


public:


private:

	unsigned int idTexture;

};