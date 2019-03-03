#pragma once

#include "Object3D.h"
#include "Wheel.h"
#include "enum.h"
#include "TextureManager.h"
#include "CarParts.h"

class Car : public Object3D
{

	friend class CarGauge;

public:

	Car();

	void importFromObjFile(const char* filePath, TextureManager* textureManager, Textures textureName, float scaleRatio = 1.0f);
	void display();

	void move();
	void accelerate();
	void slow();
	void turnRight();
	void turnLeft();
	void gearUp() { gearBox.gearUp(); engine.changeRPM(gearBox.getCurrentTransmission(), gearBox.getNextTransmission()); };
	void gearDown() { gearBox.gearDown(); };

	Point getCameraCenter();
	Point getCameraLookAt();

	float getVelocity() { return v; } //[m/s]
	float getRPM() { return engine.getRPM(); } //[m/s]
	unsigned int getCurrentGear() { return gearBox.getCurrentGear(); } //[-]

private:


public:


private:

	float a = 0;
	float v = 0;
	float mass = 1000; //[kg]
	float rd = 0.3; //[m]
	float vMax = 150.0 / 3.6; //[m / s]
	float resistanceRatio; //[kN / (m / s)^2]
	float nm = 0.9; //[-]

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

	Engine engine;
	GearBox gearBox;
};