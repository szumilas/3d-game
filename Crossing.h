#pragma once

#include "Footway.h"

class Crossing : public Footway
{
public:

	Crossing(MapObject& mapObject);

	void calculateFinalGeometry();


};