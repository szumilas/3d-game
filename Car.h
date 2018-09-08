#pragma once

#include "Object3D.h"

class Car : public Object3D
{

public:

	void move();
	void accelerate();
	void slow();
	void turnRight();
	void turnLeft();
private:


public:


private:

	float a;
	float v;

	float v_max = 0.15;

};