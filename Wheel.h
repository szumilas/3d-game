#pragma once

#include "Object3D.h"

class Wheel : public Object3D
{
public:

	Wheel();

	void loadModel();
	void adjustPosition(float Xcar, float Ycar, float rzCar, float xOffset, float yOffset);

private:


public:


private:

};