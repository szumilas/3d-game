#pragma once

#include <allegro5/allegro_acodec.h>
#include <list>

#include "Object3D.h"
#include "Wheel.h"
#include "enum.h"
#include "TextureManager.h"
#include "CarParts.h"
#include "PacejkaModel.h"
#include "ObjectWithCamera.h"

#include "carDB.h"

class Car : public Object3D, public ObjectWithCamera
{

	friend class CarGauge;
	friend class Screen2D;
	friend class RaceTimer;


public:

	//void setObstacle(float x, float y)
	//{
	//	AIdirection.x = x;
	//	AIdirection.y = y;
	//}

	void setObstacleVelocity(float x, float y)
	{
		//obstacleV.x = x - obstacle.center.x;
		//obstacleV.y = y - obstacle.center.y;
	}



	Car(CarBrand carBrand, float startX, float startY, bool humanCar = false);
	~Car();

	void display();

	void update();
	void move();
	void updateGhostCarAbility();
	void accelerate();
	void slow();
	void breakPressed();
	void stop() { a = 0; v = { 0, 0 }; velocity = 0; angularVelocity = 0; pacejkaModel.stop(); }
	void setAIcurrentPoint(int newAIcurrentPoint) { AIcurrentPoint = newAIcurrentPoint; }
	int getAIcurrentPoint() { return AIcurrentPoint; }
	void turnRight();
	void turnLeft();
	void gearUp() { gearBox.gearUp(); engine.changeRPM(gearBox.getCurrentTransmission(), gearBox.getNextTransmission()); };
	void gearDown() { gearBox.gearDown(); };
	void resetPositionToAIPath();
	void AIPathResetCounter();

	Point getCameraCenter();
	Point getCameraLookAt();

	float getVelocity() { return v.length(); } //[m/s]
	float getRPM() { return engine.getRPM(); } //[m/s]
	CarBrand getCarBrand() const { return carBrand; }
	char getCurrentGearTxt() { return gearBox.getCurrentGearTxt(); } //[-]
	void importFromObjFile();
	void setBackCamera() { cameraCenter = Point{ -8, 0, 5 }; cameraLookAt = Point{ 0, 0, 3 }; }
	void setFrontRightCamera() { cameraCenter = Point{ 5, 2, 2 }; cameraLookAt = Point{0, 0, 1}; }
	bool isHumanCar() { return humanCar; }
	void mute();

private:

	void straightenSteeringWheelAngle();
	void calculateMovement();
	void setLastWheelPosition();
	void calculateCarDrift();
	void playEngineSound();
	void playDriftSound(bool carDrifting);
	void playCrashSound(const vector2D& modelCircleGlobalVelocity);
	void calculateNetForces();
	void calculateCollisions();
	int drivingDirection();
	void AImove();
	void findAproximateAIPathPosition();

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
	int humanCar = 0;
	int AIcurrentPoint = 0;

	int AIcurrentPointTimer = 999999999;

	bool ghostCar = false;
	int ghostCarTimer = 0;
};