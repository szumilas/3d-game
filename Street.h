#pragma once

#include "MapObject.h"

class Street : public MapObject
{
public:

	Street(MapObject& mapObject) : MapObject(mapObject) {};

	void calculateFinalGeometry(TextureManager* textureManager);

private:


public:


private:

	unsigned int idTexture;

};