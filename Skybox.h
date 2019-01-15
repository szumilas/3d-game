#pragma once

#include "MapObject.h"

class Skybox : public MapObject
{
public:
	
	Skybox(TextureManager* textureManager);
	void calculateXYfromRef(const std::map<long long, node> &nodes);
	void calculateFinalGeometry(TextureManager* textureManager);

private:


public:


private:


};