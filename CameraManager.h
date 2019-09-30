#pragma once

#include <memory>
#include "GlobalStructures.h"
#include "Orbit.h"
#include "ObjectWithCamera.h"

class CameraManager
{
public:

	CameraManager(ObjectWithCamera* orbit);
	static void Init(Orbit* orbit);
	static std::unique_ptr<CameraManager>& Instance();

	std::pair<Point, Point> getCurrentCameraPoints();
	void updateSpecialCameraPathPosition();

	void adjustCamera(int idCameraView);

private:


public:

	Point center;
	Point lookAt;

	std::vector<ObjectWithCamera*> cameraViews;

private:

	float idSpecialCameraPath = 0;
	std::vector<std::pair<Point, Point>> specialCameraPath{
		{ { 0, 0, 10 },{ 10, 10, 0 } },
		{ { 5, 5, 7.5 },{ 12.5, 12.5, 0 } },
		{ { 10, 10, 5 },{ 15, 15, 0 } },
	};

	static std::unique_ptr<CameraManager> _instance;

};