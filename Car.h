#pragma once

#include "Object3D.h"
#include "Wheel.h"

class Car : public Object3D
{

public:

	Car();

	void loadModel();

	void move();
	void accelerate();
	void slow();
	void turnRight();
	void turnLeft();

	void printWheels();

private:


public:


private:

	float a;
	float v;

	float v_max = 0.15;

	std::vector<Wheel> wheels;
	float wheelBaseOffset;
	float wheelBase;
	float track;

};