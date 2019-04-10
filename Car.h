#pragma once

#include <allegro5/allegro_acodec.h>
#include <list>

#include "Object3D.h"
#include "Wheel.h"
#include "enum.h"
#include "TextureManager.h"
#include "CarParts.h"

#include "carDB.h"

class Car : public Object3D
{

	friend class CarGauge;
	friend class Screen2D;


public:

	Car(CarBrand carBrand, float startX, float startY, Point* globalCameraCenter, Point* globalCameraLookAt);

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

	void straightenSteeringWheelAngle();
	void calculateMovement();
	void setLastWheelPosition();
	void calculateCarDrift();
	void playEngineSound();

	template <typename T> int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	Point getCarPointInGlobalSystem(const Point& p)
	{
		return { position.x + cos(rz) * p.x - sin(rz) * p.y, position.y + sin(rz) * p.x + cos(rz) * p.y };
	}

public:

	float resultantTorque;

private:

	CarBrand carBrand;

	Wheel backWheels;
	Wheel leftWheel;
	Wheel rightWheel;

	Point lastWheelsPosition[4];
	std::list<Point> wheelsTrace;

	float a = 0;
	float v = 0; //[m/s]
	float vx = 0; //[m/s]
	float vy = 0; //[m/s]
	float mass; //[kg]
	float rd = 0.3; //[m]
	float vMax; //[m / s]
	float resistanceRatio; //[kN / (m / s)^2]
	float nm = 0.8; //[-]

	vector2D vLoc{0.0, 0.0};

	float steeringWheelAngle;

	float acceleration; //[m/s^s]

	bool tryAccelerate;
	bool trySlow;

	std::vector<Wheel> wheels;
	float frontWheelsYoffset;
	float frontWheelsXoffset;
	float backWheelsXoffset;
			
	Point cameraCenter;
	Point cameraLookAt;

	Point* globalCameraCenter;
	Point* globalCameraLookAt;

	Engine engine;
	GearBox gearBox;

	std::vector<Force> forces;

	ALLEGRO_SAMPLE_INSTANCE* engineSound;
};