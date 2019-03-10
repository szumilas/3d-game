#pragma once

#include "Object3D.h"

class Wheel : public Object3D
{
public:

	Wheel();

	void display();
	void calculateGeometry(float newXoffset = 0.0f, float newYoffset = 0.0f);
	void setCarPosition(float newX, float nexY, float newZ) { carX = newX; carY = nexY; carZ = newZ; }
	void setCarAngle(float rz) { car_sin_rz = sin(rz); car_cos_rz = cos(rz); }
	void rotate(float v);

private:


public:


private:

	float r = 0.0f;
	float xOffset;
	float yOffset;
	float carX;
	float carY;
	float carZ;
	float car_sin_rz;
	float car_cos_rz;
	float angle = 0.0f;
};