#include "CameraManager.h"

std::unique_ptr<CameraManager> CameraManager::_instance;

CameraManager::CameraManager(ObjectWithCamera* orbit)
{
	cameraViews.push_back(orbit);

	center.x = 15;
	center.y = -30;
	center.z = 10;
}

void CameraManager::Init(Orbit* orbit)
{
	_instance = std::unique_ptr<CameraManager>(new CameraManager(orbit));
}

std::unique_ptr<CameraManager>& CameraManager::Instance()
{
	return _instance;
}

std::pair<Point, Point> CameraManager::getCurrentCameraPoints()
{
	return { center, lookAt };
}

void CameraManager::adjustCamera(int idCameraView)
{
	center = cameraViews[idCameraView]->getCameraCenter();
	lookAt = cameraViews[idCameraView]->getCameraLookAt();
}