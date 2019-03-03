#pragma once

#include "MapObject.h"

class Water : public MapObject
{
public:

	Water(MapObject& mapObject) : MapObject(mapObject) {};

	void calculateFinalGeometry();

private:


public:


private:

};