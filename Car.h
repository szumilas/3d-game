#pragma once

#include "Object3D.h"
#include "Wheel.h"
#include "enum.h"
#include "TextureManager.h"

class Car : public Object3D
{

public:

	Car();

	void loadModel();
	void importFromObjFile(const char* filePath, TextureManager* textureManager);
	void display();

	void move();
	void accelerate();
	void slow();
	void turnRight();
	void turnLeft();

	void printWheels();

	Point getCameraCenter();
	Point getCameraLookAt();

private:


public:


private:

	float a;
	float v;

	float steeringWheelAngle;

	float v_max = 60; //[m/s]
	float acceleration = 6; //[m/s^s]

	bool tryAccelerate;
	bool trySlow;

	std::vector<Wheel> wheels;
	float wheelBaseOffset;
	float wheelBase;
	float track;

	Point cameraCenter;
	Point cameraLookAt;

};