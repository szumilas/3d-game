#pragma once

#include "MapObject.h"

class Street : public MapObject
{
public:

	Street(MapObject& mapObject) : MapObject(mapObject) {};

	void calculateFinalGeometry();

private:


public:


private:

	unsigned int idTexture;

};