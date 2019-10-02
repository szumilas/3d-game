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
	if (idCameraView >=0 && idCameraView < cameraViews.size())
	{
		center = cameraViews[idCameraView]->getCameraCenter();
		lookAt = cameraViews[idCameraView]->getCameraLookAt();
	}
	else
	{
		center = specialCameraPath[static_cast<int>(idSpecialCameraPath)].first;
		lookAt = specialCameraPath[static_cast<int>(idSpecialCameraPath)].second;
	}
}

void CameraManager::updateSpecialCameraPathPosition()
{
	idSpecialCameraPath += 30 / FPS;

	if (idSpecialCameraPath >= specialCameraPath.size())
	{
		idSpecialCameraPath -= (specialCameraPath.size());
	}
}