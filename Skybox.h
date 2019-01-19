#pragma once

#include "MapObject.h"

class Skybox : public MapObject
{
public:
	
	Skybox(TextureManager* textureManager, float minX, float maxX, float minY, float maxY);
	void calculateXYfromRef();
	void calculateFinalGeometry(TextureManager* textureManager);

private:


public:


private:


};