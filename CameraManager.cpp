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
		if (carZero == nullptr)
		{
			center = specialCameraPath[static_cast<int>(idSpecialCameraPath)].first;
			lookAt = specialCameraPath[static_cast<int>(idSpecialCameraPath)].second;
		}
		else
		{
			center = specialCameraPath[static_cast<int>(idSpecialCameraPath)].first;
			Point::rotate(center, Point(), carZero->rz);
			center += carZero->position;

			lookAt = specialCameraPath[static_cast<int>(idSpecialCameraPath)].second;
			Point::rotate(lookAt, Point(), carZero->rz);
			lookAt += carZero->position;
		}
	}
}

bool CameraManager::updateSpecialCameraPathPosition()
{
	idSpecialCameraPath += specialCameraSpeed / FPS;

	if (idSpecialCameraPath >= specialCameraPath.size())
	{
		idSpecialCameraPath = 0;
		return false;
	}
	return true;
}