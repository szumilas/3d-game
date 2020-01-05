#pragma once

#include "MapObject.h"

class Skybox : public MapObject
{
public:
	
	Skybox(float minX, float maxX, float minY, float maxY);
	void calculateXYfromRef();
	void calculateFinalGeometry();

private:


public:


private:

	float skyboxHeight = 1500;

};