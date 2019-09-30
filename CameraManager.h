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

	void adjustCamera(int idCameraView);

private:


public:

	Point center;
	Point lookAt;

	std::vector<ObjectWithCamera*> cameraViews;

private:

	static std::unique_ptr<CameraManager> _instance;

};