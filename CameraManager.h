#pragma once

#include <memory>
#include "GlobalStructures.h"
#include "Orbit.h"
#include "ObjectWithCamera.h"
#include "Car.h"

class CameraManager
{
public:

	CameraManager(ObjectWithCamera* orbit);
	static void Init(Orbit* orbit);
	static std::unique_ptr<CameraManager>& Instance();

	std::pair<Point, Point> getCurrentCameraPoints();
	bool updateSpecialCameraPathPosition();

	void adjustCamera(int idCameraView);
	void restartIdSpecialCameraPath() { idSpecialCameraPath = 0; };
	void setCarZero(Car* car) { carZero = car; }
	Car* getCarZero() { return carZero; }

	void increaseSpecialCameraSpeed() { specialCameraSpeed *= 1.1; }
	void decreaseSpecialCameraSpeed() { specialCameraSpeed /= 1.1; }

private:


public:

	Point center;
	Point lookAt;

	std::vector<ObjectWithCamera*> cameraViews;
	std::vector<std::pair<Point, Point>> specialCameraPath;

private:

	float idSpecialCameraPath = 0;
	Car* carZero = nullptr;
	double specialCameraSpeed = 50;

	static std::unique_ptr<CameraManager> _instance;

};