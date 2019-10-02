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
	void restartIdSpecialCameraPath() { idSpecialCameraPath = 0; };

private:


public:

	Point center;
	Point lookAt;

	std::vector<ObjectWithCamera*> cameraViews;
	std::vector<std::pair<Point, Point>> specialCameraPath;

private:

	float idSpecialCameraPath = 0;

	static std::unique_ptr<CameraManager> _instance;

};