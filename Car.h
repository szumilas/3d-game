#pragma once

#include "Object3D.h"
#include "Wheel.h"
#include "enum.h"
#include "TextureManager.h"
#include "CarParts.h"

#include "carDB.h"

class Car : public Object3D
{

	friend class CarGauge;

public:

	Car(CarBrand carBrand, float startX, float startY);

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
	void importFromObjFile();

private:


public:


private:

	CarBrand carBrand;

	Wheel backWheels;
	Wheel leftWheel;
	Wheel rightWheel;

	float a = 0;
	float v = 0; //[m/s]
	float mass; //[kg]
	float rd = 0.3; //[m]
	float vMax; //[m / s]
	float resistanceRatio; //[kN / (m / s)^2]
	float nm = 0.8; //[-]

	float steeringWheelAngle;

	float acceleration; //[m/s^s]

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