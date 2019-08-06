#pragma once

#include <allegro5/allegro_acodec.h>
#include <list>

#include "Object3D.h"
#include "Wheel.h"
#include "enum.h"
#include "TextureManager.h"
#include "CarParts.h"
#include "PacejkaModel.h"

#include "carDB.h"

class Car : public Object3D
{

	friend class CarGauge;
	friend class Screen2D;


public:

	void setObstacle(float x, float y)
	{
		//obstacle.center.x = x;
		//obstacle.center.y = y;
	}

	void setObstacleVelocity(float x, float y)
	{
		//obstacleV.x = x - obstacle.center.x;
		//obstacleV.y = y - obstacle.center.y;
	}

	vector2D obstacleV;
	float obstacleMass = 1500;

	//Circle obstacle{ {0.0f, 0.0f, 0.05f }, 1.0f };

	Car(CarBrand carBrand, float startX, float startY, Point* globalCameraCenter, Point* globalCameraLookAt, bool humanCar = false);

	void display();

	void move();
	void accelerate();
	void slow();
	void breakPressed();
	void stop() { a = 0; v = { 0, 0 }; velocity = 0; angularVelocity = 0; }
	void turnRight();
	void turnLeft();
	void gearUp() { gearBox.gearUp(); engine.changeRPM(gearBox.getCurrentTransmission(), gearBox.getNextTransmission()); };
	void gearDown() { gearBox.gearDown(); };

	Point getCameraCenter();
	Point getCameraLookAt();

	float getVelocity() { return v.length(); } //[m/s]
	float getRPM() { return engine.getRPM(); } //[m/s]
	char getCurrentGearTxt() { return gearBox.getCurrentGearTxt(); } //[-]
	void importFromObjFile();

private:

	void straightenSteeringWheelAngle();
	void calculateMovement();
	void setLastWheelPosition();
	void calculateCarDrift();
	void playEngineSound();
	void playDriftSound(bool carDrifting);
	void calculateNetForces();
	void calculateCollisions();
	int drivingDirection();

	template <typename T> int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	Point getCarPointInGlobalSystem(const Point& p)
	{
		return { position.x + cos(rz) * p.x - sin(rz) * p.y, position.y + sin(rz) * p.x + cos(rz) * p.y };
	}

public:

private:

	CarBrand carBrand;

	Wheel backWheels;
	Wheel leftWheel;
	Wheel rightWheel;

	Point lastWheelsPosition[4];
	std::list<Point> wheelsTrace;

	float a = 0;
	vector2D v{0.0, 0}; //[m/s] local
	vector2D acceleration{ 0.0, 0.0 }; //[m/s^s] local
	float velocity = 0; //[m/s]
	float angularVelocity = 0; //[rad/s]
	float mass; //[kg]
	float momentOfInertia; //[kg * m^2]
	float rd = 0.3; //[m]
	float vMax; //[m / s]
	float resistanceRatio; //[kN / (m / s)^2]
	float nm = 0.8; //[-]

	int drivingDir = 0;

	float steeringWheelAngle;


	bool tryAccelerate = false;
	bool trySlow = false;
	bool tryBreak = false;
	bool turning = false;

	std::vector<Wheel> wheels;
	float frontWheelsYoffset;
	float frontWheelsXoffset;
	float backWheelsXoffset;

	float width;
	float length;

	std::vector<Circle> carModelCircles;
	std::vector<Point> temporary;
				
	Point cameraCenter;
	Point cameraLookAt;

	Point* globalCameraCenter;
	Point* globalCameraLookAt;

	Engine engine;
	GearBox gearBox;

	std::vector<Force> forces;
	Force netForce; //[N]
	float netTorque; //[N * m]

	ALLEGRO_SAMPLE_INSTANCE* engineSound;
	ALLEGRO_SAMPLE_INSTANCE* driftSound;

	PacejkaModel pacejkaModel;
	bool humanCar = false;
};