#pragma once

#include "MapObject.h"

class GreenArea : public MapObject
{
public:

	GreenArea(MapObject& mapObject) : MapObject(mapObject) {};

	void calculateFinalGeometry();

private:


public:


private:

	unsigned int idTexture;

};