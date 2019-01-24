#pragma once

#include "Building.h"

class PasazGrunwaldzki : public Building
{

public:

	PasazGrunwaldzki(MapObject& mapObject);
	void calculateFinalGeometry(TextureManager* textureManager);

private:

	void generateWalls();

};