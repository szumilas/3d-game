#pragma once

#include "Building.h"

class GrunwaldzkiCenter : public Building
{

public:

	GrunwaldzkiCenter(MapObject& mapObject);
	void calculateFinalGeometry(TextureManager* textureManager);

private:

	void generateWalls();

};