#include "MapContainer.h"

#include "CameraManager.h"
#include "MapManager.h"
#include "MetaLine.h"
#include <fstream>
#include <algorithm>
#include "KeyboardManager.h"

std::vector<std::vector<std::vector<std::unique_ptr<MapObject>*>>> MapContainer::mapObjectSections;
std::vector<std::vector<std::vector<std::unique_ptr<MapObject>*>>> MapContainer::mapCollidableObjectSections;
std::vector<std::unique_ptr<MapObject>*> MapContainer::highBuildingSection;
std::unique_ptr<MapObject>* MapContainer::background;
std::vector<std::unique_ptr<MapObject>*> MapContainer::ground;
std::vector<std::unique_ptr<MapObject>> MapContainer::extraObjects;
float MapContainer::deltaX;
float MapContainer::deltaY;
float MapContainer::maxX;
float MapContainer::minX;
float MapContainer::maxY;
float MapContainer::minY;
std::unique_ptr<MapContainer> MapContainer::_instance;
std::vector<Car> MapContainer::cars;
std::vector<MapContainer::PathStruct> MapContainer::currentPath;
std::vector<MapContainer::PathStruct> MapContainer::AIPoints;
std::vector<std::pair<Point, Point>> MapContainer::raceBarriers;
std::pair<Point, Point> MapContainer::meta;
int MapContainer::lapRecord;
MapContainer::SplineStruct MapContainer::currentSpline;
std::pair<Spline, Spline> MapContainer::cameraSpline;
std::vector<float> MapContainer::AIPointsDistances;
RaceTimer MapContainer::raceTimer;
int MapContainer::w;
int MapContainer::h;
int MapContainer::mapEditorSelectedPanel = 0;
int MapContainer::mapEditorSelectedTool = 0;
int MapContainer::currentToolId = MapContainer::e_AddPoint;
std::vector<std::vector<int>> MapContainer::tools = MapContainer::createTools();
std::map<int, void (MapContainer::*)(const Point&)> MapContainer::toolsMap = createToolsMap();
Color MapContainer::AIPointsColor = Color(ColorName::ORANGE);
Color MapContainer::pointsColor = Color(ColorName::BLUE);
Color MapContainer::splinePointsColor = Color(ColorName::PINK);
Color MapContainer::splineSubointsColor = Color(ColorName::BLACK);
bool MapContainer::mapEditorPanelVisible = true;
bool MapContainer::linesVisible = true;
bool MapContainer::cursorsVisible = true;
double MapContainer::displayWorldCameraHeight = 0;
int MapContainer::trailerScenery = 0;

std::vector<std::vector<int>> MapContainer::createTools()
{
	std::vector<std::vector<int>> tools;

	std::vector<int> PathTools{
		MapContainer::e_AddPoint,
		MapContainer::e_AddPointInTheMiddle,
		MapContainer::e_RemovePoint,
		MapContainer::e_MovePoint,
		MapContainer::e_SelectPoint,
		MapContainer::e_RemovePoints,
		MapContainer::e_RandomizePoints,
		MapContainer::e_AddSplinePoint,
		MapContainer::e_RemoveSplinePoint,
		MapContainer::e_MoveSplinePoint,
		MapContainer::e_SelectSplinePoint,
		MapContainer::e_RemoveSplinePoints,
		MapContainer::e_DivideSpline,
		MapContainer::e_IncreaseSplineSubpoints,
		MapContainer::e_DecreaseSplineSubpoints,
		MapContainer::e_ConvertSplineToCurrentPath,
		MapContainer::e_ConvertSplinePointsToCurrentPath,
	};
	tools.push_back(PathTools);

	std::vector<int> AITools{
		MapContainer::e_LoadAIPoints,
		MapContainer::e_SaveAIPoints,
		MapContainer::e_ConvertPathToAIPoints,
		MapContainer::e_ConvertAIPointsToPath,
		MapContainer::e_AIPlay,
		MapContainer::e_AIPause,
		MapContainer::e_AIStop,
		MapContainer::e_AIStopAndRestartToSelectedPoint,
		MapContainer::e_StartRace,
		MapContainer::e_ConvertPathToRaceBarriers,
		MapContainer::e_ConvertPathToMeta,
		MapContainer::e_ConvertPathToRaceBarriersSaveAndClearPath,
	};
	tools.push_back(AITools);

	std::vector<int> CameraTools{
		MapContainer::e_AddCameraPoint,
		MapContainer::e_RemoveCameraPoints,
		MapContainer::e_ConvertCameraPointsToSpline,
		MapContainer::e_ConvertCameraPointsToSplineVariousDistances,
		MapContainer::e_PlayCameraSpline,
		MapContainer::e_ConvertCameraSplineToCarZero,
		MapContainer::e_PlayCameraSplineAroundCarZero,
		MapContainer::e_SaveCameraSpline,
		MapContainer::e_LoadCameraSpline,
		MapContainer::e_IncreaseSpecialCameraSpeed,
		MapContainer::e_DecreaseSpecialCameraSpeed,
	};
	tools.push_back(CameraTools);

	std::vector<int> NewObjectTools{
		MapContainer::e_SaveNewObject,
		MapContainer::e_DeleteLastNewObject,
		MapContainer::e_AddTree,
		MapContainer::e_AddStreetLamp,
		MapContainer::e_AddBuilding,
		MapContainer::e_AddBarrier,
		MapContainer::e_AddRamp,
		MapContainer::e_AddCustomArea,
	};
	tools.push_back(NewObjectTools);

	std::vector<int> NewObjectSettingsTools{
		MapContainer::e_NewObjectAddHeight,
		MapContainer::e_NewObjectReduceHeight,
		MapContainer::e_NewObjectAddMinHeight,
		MapContainer::e_NewObjectReduceMinHeight,
		MapContainer::e_NewObjectAddRed,
		MapContainer::e_NewObjectReduceRed,
		MapContainer::e_NewObjectAddGreen,
		MapContainer::e_NewObjectReduceGreen,
		MapContainer::e_NewObjectAddBlue,
		MapContainer::e_NewObjectReduceBlue,
		MapContainer::e_NewObjectAddRemoveCustomTexture,
		MapContainer::e_NewObjectNextTextrue,
	};
	tools.push_back(NewObjectSettingsTools);

	std::vector<int> MapObjectSettingsTools{
		MapContainer::e_ReloadMap,
		MapContainer::e_SaveMapObject,
		MapContainer::e_EditMapObject,
		MapContainer::e_EditMapObjectNextBuildingTexture,
		MapContainer::e_EditMapObjectNextRoofTexture,
		MapContainer::e_EditMapObjectDoNotGenerateBottomRoof,
		MapContainer::e_EditMapObjectChangeRoofType,
		MapContainer::e_EditMapObjectAddHeight,
		MapContainer::e_EditMapObjectReduceHeight,
		MapContainer::e_EditMapObjectAddRed,
		MapContainer::e_EditMapObjectReduceRed,
		MapContainer::e_EditMapObjectAddGreen,
		MapContainer::e_EditMapObjectReduceGreen,
		MapContainer::e_EditMapObjectAddBlue,
		MapContainer::e_EditMapObjectReduceBlue,
	};
	tools.push_back(MapObjectSettingsTools);
	

	tools.push_back({});

	return tools;
}

std::map<int, void (MapContainer::*)(const Point&)> MapContainer::createToolsMap()
{
	std::map<int, void (MapContainer::*)(const Point&)> map = {
	
		{ e_AddPoint, &MapContainer::addPoint },
		{ e_AddPointInTheMiddle, &MapContainer::addPointInTheMiddle },
		{ e_RemovePoint, &MapContainer::removePoint },
		{ e_MovePoint, &MapContainer::movePoint },
		{ e_SelectPoint, &MapContainer::selectPoint },
		{ e_SaveAIPoints, &MapContainer::SaveAIPoints },
		{ e_LoadAIPoints, &MapContainer::LoadAIPoints },
		{ e_AIPlay, &MapContainer::AIPlay },
		{ e_AIPause, &MapContainer::AIPause },
		{ e_AIStop, &MapContainer::AIStop },
		{ e_AIStopAndRestartToSelectedPoint, &MapContainer::AIStopAndRestartToSelectedPoint },
		{ e_StartRace, &MapContainer::startRace },
		{ e_ConvertPathToAIPoints, &MapContainer::ConvertPathToAIPoints },
		{ e_ConvertAIPointsToPath, &MapContainer::ConvertAIPointsToPath },
		{ e_RemovePoints, &MapContainer::removePoints },
		{ e_RandomizePoints, &MapContainer::randomizePoints },
		{ e_AddSplinePoint, &MapContainer::AddSplinePoint },
		{ e_RemoveSplinePoint, &MapContainer::RemoveSplinePoint },
		{ e_MoveSplinePoint, &MapContainer::MoveSplinePoint },
		{ e_SelectSplinePoint, &MapContainer::SelectSplinePoint },
		{ e_RemoveSplinePoints, &MapContainer::RemoveSplinePoints },
		{ e_DivideSpline, &MapContainer::DivideSpline },
		{ e_IncreaseSplineSubpoints, &MapContainer::IncreaseSplineSubpoints },
		{ e_DecreaseSplineSubpoints, &MapContainer::DecreaseSplineSubpoints },
		{ e_ConvertSplineToCurrentPath, &MapContainer::ConvertSplineToCurrentPath },
		{ e_ConvertSplinePointsToCurrentPath, &MapContainer::ConvertSplinePointsToCurrentPath },
		{ e_ConvertPathToRaceBarriers, &MapContainer::ConvertPathToRaceBarriers },
		{ e_ConvertPathToMeta, &MapContainer::ConvertPathToMeta },
		{ e_ConvertPathToRaceBarriersSaveAndClearPath, &MapContainer::ConvertPathToRaceBarriersSaveAndClearPath },

		{ e_AddCameraPoint, &MapContainer::AddCameraPoint },
		{ e_RemoveCameraPoints, &MapContainer::RemoveCameraPoints },
		{ e_ConvertCameraPointsToSpline, &MapContainer::ConvertCameraPointsToSpline },
		{ e_ConvertCameraPointsToSplineVariousDistances, &MapContainer::ConvertCameraPointsToSplineVariouosDistances },
		{ e_PlayCameraSpline, &MapContainer::PlayCameraSpline },
		{ e_ConvertCameraSplineToCarZero, &MapContainer::ConvertCameraSplineToCarZero },
		{ e_PlayCameraSplineAroundCarZero, &MapContainer::PlayCameraSplineAroundCarZero },
		{ e_SaveCameraSpline, &MapContainer::SaveCameraSpline },
		{ e_LoadCameraSpline, &MapContainer::LoadCameraSpline },
		{ e_IncreaseSpecialCameraSpeed, &MapContainer::IncreaseSpecialCameraSpeed },
		{ e_DecreaseSpecialCameraSpeed, &MapContainer::DecreaseSpecialCameraSpeed },

		{ e_SaveNewObject, &MapContainer::SaveNewObject },
		{ e_DeleteLastNewObject, &MapContainer::DeleteLastNewObject },
		{ e_AddTree, &MapContainer::AddTree },
		{ e_AddStreetLamp, &MapContainer::AddStreetLamp },
		{ e_AddBuilding, &MapContainer::AddBuilding },
		{ e_AddBarrier, &MapContainer::AddBarrier },
		{ e_AddRamp, &MapContainer::AddRamp },
		{ e_AddCustomArea, &MapContainer::AddCustomArea },

		{ e_NewObjectAddHeight, &MapContainer::NewObjectAddHeight },
		{ e_NewObjectReduceHeight, &MapContainer::NewObjectReduceHeight },
		{ e_NewObjectAddMinHeight, &MapContainer::NewObjectAddMinHeight },
		{ e_NewObjectReduceMinHeight, &MapContainer::NewObjectReduceMinHeight },
		{ e_NewObjectAddRed, &MapContainer::NewObjectAddRed },
		{ e_NewObjectReduceRed, &MapContainer::NewObjectReduceRed },
		{ e_NewObjectAddGreen, &MapContainer::NewObjectAddGreen },
		{ e_NewObjectReduceGreen, &MapContainer::NewObjectReduceGreen },
		{ e_NewObjectAddBlue, &MapContainer::NewObjectAddBlue },
		{ e_NewObjectReduceBlue, &MapContainer::NewObjectReduceBlue },
		{ e_NewObjectAddRemoveCustomTexture, &MapContainer::NewObjectAddRemoveCustomTexture },
		{ e_NewObjectNextTextrue, &MapContainer::NewObjectNextTexture },

		{ e_ReloadMap, &MapContainer::ReloadMap },
		{ e_SaveMapObject, &MapContainer::SaveMapObject },
		{ e_EditMapObject, &MapContainer::EditMapObject },
		{ e_EditMapObjectNextBuildingTexture, &MapContainer::EditMapObjectNextBuildingTexture },
		{ e_EditMapObjectNextRoofTexture, &MapContainer::EditMapObjectNextRoofTexture },
		{ e_EditMapObjectDoNotGenerateBottomRoof, &MapContainer::EditMapObjectDoNotGenerateBottomRoof },
		{ e_EditMapObjectChangeRoofType, &MapContainer::EditMapObjectChangeRoofType },
		{ e_EditMapObjectAddHeight, &MapContainer::EditMapObjectAddHeight },
		{ e_EditMapObjectReduceHeight, &MapContainer::EditMapObjectReduceHeight },
		{ e_EditMapObjectAddRed, &MapContainer::EditMapObjectAddRed },
		{ e_EditMapObjectReduceRed, &MapContainer::EditMapObjectReduceRed },
		{ e_EditMapObjectAddGreen, &MapContainer::EditMapObjectAddGreen },
		{ e_EditMapObjectReduceGreen, &MapContainer::EditMapObjectReduceGreen },
		{ e_EditMapObjectAddBlue, &MapContainer::EditMapObjectAddBlue },
		{ e_EditMapObjectReduceBlue, &MapContainer::EditMapObjectReduceBlue },

	};

	return map;
}




void MapContainer::Init(int w, int h)
{
	MapContainer::w = w;
	MapContainer::h = h;
	_instance = std::unique_ptr<MapContainer>(new MapContainer());
}

std::unique_ptr<MapContainer>& MapContainer::Instance()
{
	return _instance;
}

std::vector<std::vector<std::unique_ptr<MapObject>*>*> MapContainer::getCollidableObjectsInPosition(const Point& position)
{
	std::vector<std::vector<std::unique_ptr<MapObject>*>*> result;

	int yToReturn = static_cast<int>(100 * (position.y - minY) / deltaY);
	int xToReturn = static_cast<int>(100 * (position.x - minX) / deltaX);

	if (yToReturn >= 100 || xToReturn >= 100 || yToReturn < 0 || xToReturn < 0)
		return result;

	result.push_back(&mapCollidableObjectSections[xToReturn][yToReturn]);
	if(yToReturn > 0)
		result.push_back(&mapCollidableObjectSections[xToReturn][yToReturn - 1]);
	if (yToReturn < 99)
		result.push_back(&mapCollidableObjectSections[xToReturn][yToReturn + 1]);
	if (xToReturn > 0)
		result.push_back(&mapCollidableObjectSections[xToReturn - 1][yToReturn]);
	if (xToReturn < 99)
		result.push_back(&mapCollidableObjectSections[xToReturn + 1][yToReturn]);

	if (xToReturn > 0 && yToReturn > 0)
		result.push_back(&mapCollidableObjectSections[xToReturn - 1][yToReturn - 1]);
	if (xToReturn > 0 && yToReturn < 99)
		result.push_back(&mapCollidableObjectSections[xToReturn - 1][yToReturn + 1]);
	if (xToReturn < 99 && yToReturn > 0)
		result.push_back(&mapCollidableObjectSections[xToReturn + 1][yToReturn - 1]);
	if (xToReturn < 99 && yToReturn < 99)
		result.push_back(&mapCollidableObjectSections[xToReturn + 1][yToReturn + 1]);

	return result;
}

long long MapContainer::shootRay(std::vector<std::pair<int, int>>& sectionsToDisplay, Point& center, vector2D& currentRay)
{
	long long collisionId = -1;

	int distanceUnit = 5;
	bool rayFinished = false;
	
	Color colorOfTheRay(ColorName::BLACK);
	
	for(; distanceUnit < 1500; distanceUnit += 5)
	{
		Point currenPoint(center.x + currentRay.x * distanceUnit, center.y + currentRay.y * distanceUnit);
		int sectionX = static_cast<int>(100 * (currenPoint.x - minX) / deltaX);
		int sectionY = static_cast<int>(100 * (currenPoint.y - minY) / deltaY);
	
		if (sectionX >= 100 || sectionY >= 100 ||
			sectionX < 0 || sectionY < 0)
		{
			break;
		}
		else
		{
			sectionsToDisplay.push_back({ sectionX, sectionY });
		}
		
		for (auto& object : mapObjectSections[sectionsToDisplay.back().first][sectionsToDisplay.back().second])
		{
			if (!object->get()->building.empty() && object->get()->_min_height == 0 && object->get()->_height >= displayWorldCameraHeight + 5)
			{
				if (PointInsidePolygonDetector::isInside(object->get()->points, currenPoint))
				{
					rayFinished = true;
					colorOfTheRay = object->get()->getUniqueColorBasedOnId();
					collisionId = object->get()->getId();
					break;
				}
			}
		}
	
		if (rayFinished)
			break;
	}
	
	//glBegin(GL_LINES);
	//glColor3f(colorOfTheRay.red, colorOfTheRay.green, colorOfTheRay.blue);
	//glVertex3f(center.x, center.y, 2.0f);
	//glVertex3f(center.x + currentRay.x * distanceUnit, center.y + currentRay.y * distanceUnit, 2.0f);
	//glEnd();

	return collisionId;
}

void MapContainer::checkMiddleRay(vector2D& firstRay, long long firstRayCollisionId, vector2D& lastRay, long long lastRayCollisionId, std::vector<std::pair<int, int>>& sectionsToDisplay, Point& center)
{
	if (firstRayCollisionId > 0 && firstRayCollisionId == lastRayCollisionId || vector2D::angle(firstRay, lastRay) < PI / 72 || firstRayCollisionId < 0 && firstRayCollisionId == lastRayCollisionId && vector2D::angle(firstRay, lastRay) < PI / 18)
		return;

	auto middleRay = firstRay;

	middleRay.rotate(-vector2D::angle(firstRay, lastRay) / 2);

	auto middleRayCollisionId = shootRay(sectionsToDisplay, center, middleRay);
	checkMiddleRay(firstRay, firstRayCollisionId, middleRay, middleRayCollisionId, sectionsToDisplay, center);
	checkMiddleRay(middleRay, middleRayCollisionId, lastRay, lastRayCollisionId, sectionsToDisplay, center);
}

void MapContainer::displayWorld(std::pair<Point, Point>& camera)
{
	//Point& center = cars[0].getCameraCenter();
	//Point& lookAt = cars[0].getCameraLookAt();

	Point& center = camera.first;
	Point& lookAt = camera.second;

	displayWorldCameraHeight = center.z;

	displayLines();


	int yToDraw = static_cast<int>(100 * (center.y - minY) / deltaY);
	int xToDraw = static_cast<int>(100 * (center.x - minX) / deltaX);

	if (yToDraw >= 100 || xToDraw >= 100)
		return;
	
	std::vector<std::pair<int, int>> sectionsToDisplay;

	sectionsToDisplay.push_back({ xToDraw, yToDraw });

	vector2D firstRay(center, lookAt);
	firstRay.convertIntoUnitVector();
	firstRay.rotate(PI / 3);

	vector2D lastRay = firstRay;
	lastRay.rotate(- 2 * PI / 3);

	auto firstRayCollisionId = shootRay(sectionsToDisplay, center, firstRay);
	auto lastRayCollisionId = shootRay(sectionsToDisplay, center, lastRay);
	checkMiddleRay(firstRay, firstRayCollisionId, lastRay, lastRayCollisionId, sectionsToDisplay, center);


	//auto firstRay = frontRay;
	//
	//while (vector2D::angle(firstRay, frontRay) < 2 * PI / 3)
	//{
	//	int distanceUnit = 5;
	//	bool rayFinished = false;
	//
	//	Color colorOfTheRay(ColorName::BLACK);
	//
	//	for(; distanceUnit < 1500; distanceUnit += 5)
	//	{
	//		Point currenPoint(center.x + frontRay.x * distanceUnit, center.y + frontRay.y * distanceUnit);
	//		int sectionX = static_cast<int>(100 * (currenPoint.x - minX) / deltaX);
	//		int sectionY = static_cast<int>(100 * (currenPoint.y - minY) / deltaY);
	//
	//		if (sectionX >= 100 || sectionY >= 100 ||
	//			sectionX < 0 || sectionY < 0)
	//		{
	//			break;
	//		}
	//		else
	//		{
	//			sectionsToDisplay.push_back({ sectionX, sectionY });
	//		}
	//		
	//		for (auto& object : mapObjectSections[sectionsToDisplay.back().first][sectionsToDisplay.back().second])
	//		{
	//			if (!object->get()->building.empty() && object->get()->_min_height == 0 && object->get()->_height >= 10)
	//			{
	//				if (PointInsidePolygonDetector::isInside(object->get()->points, currenPoint))
	//				{
	//					rayFinished = true;
	//					colorOfTheRay = object->get()->getUniqueColorBasedOnId();
	//					break;
	//				}
	//			}
	//		}
	//
	//		if (rayFinished)
	//			break;
	//	}
	//
	//	glBegin(GL_LINES);
	//	glColor3f(colorOfTheRay.red, colorOfTheRay.green, colorOfTheRay.blue);
	//	glVertex3f(center.x, center.y, 2.0f);
	//	glVertex3f(center.x + frontRay.x * distanceUnit, center.y + frontRay.y * distanceUnit, 2.0f);
	//	glEnd();
	//
	//	frontRay.rotate(- std::max(atan(1.0 / distanceUnit), PI / 180));
	//}

	sort(sectionsToDisplay.begin(), sectionsToDisplay.end());
	sectionsToDisplay.erase(unique(sectionsToDisplay.begin(), sectionsToDisplay.end()), sectionsToDisplay.end());

	for (int q = sectionsToDisplay.size() - 1; q >= 0; --q)
	{
		int x = sectionsToDisplay[q].first;
		int y = sectionsToDisplay[q].second;

		sectionsToDisplay.push_back({ x + 1, y + 1 });
		sectionsToDisplay.push_back({ x + 1, y + 0 });
		sectionsToDisplay.push_back({ x + 1, y - 1 });
		sectionsToDisplay.push_back({ x + 0, y + 1 });
		sectionsToDisplay.push_back({ x + 0, y - 1 });
		sectionsToDisplay.push_back({ x - 1, y + 1 });
		sectionsToDisplay.push_back({ x - 1, y + 0 });
		sectionsToDisplay.push_back({ x - 1, y - 1 });
	}
	
	sort(sectionsToDisplay.begin(), sectionsToDisplay.end());
	sectionsToDisplay.erase(unique(sectionsToDisplay.begin(), sectionsToDisplay.end()), sectionsToDisplay.end());

	//std::cout << "section to display: " << sectionsToDisplay.size() << "                 \r";
	
	Building::setCameraAngleFlag(camera);

	for (auto& sectionToDisplay : sectionsToDisplay)
	{
		if (sectionToDisplay.first < 100 && sectionToDisplay.second < 100 &&
			sectionToDisplay.first >= 0 && sectionToDisplay.second >= 0)
		{
			auto dist = (xToDraw - sectionToDisplay.first) * (xToDraw - sectionToDisplay.first) + (yToDraw - sectionToDisplay.second) * (yToDraw - sectionToDisplay.second);

			MapObject::DrawingPriority priorityToSkip = MapObject::DrawingPriority::never;
			
			//if (dist < 256)
			//{
			//	priorityOfSection = MapObject::DrawingPriority::notImportant;
			//}
			if (dist > 300)
			{
				priorityToSkip = MapObject::DrawingPriority::notImportant;
			}

			for (auto& object : mapObjectSections[sectionToDisplay.first][sectionToDisplay.second])
			{
				if (object->get()->drawingPriority >= priorityToSkip)
					break;

				//updateScenery(*object->get(), sqrt(dist));
				
				if (!object->get()->isHidden)
					object->get()->display();
			}
		}
	}

	for (auto& sectionToDisplay : sectionsToDisplay)
	{
		if (sectionToDisplay.first < 100 && sectionToDisplay.second < 100 &&
			sectionToDisplay.first >= 0 && sectionToDisplay.second >= 0)
		{
			for (auto& object : mapObjectSections[sectionToDisplay.first][sectionToDisplay.second])
			{
				object->get()->alreadyPrinted = false;
			}
		}
	}

	for (auto& object : highBuildingSection)
	{
		if (!object->get()->isHidden)
			object->get()->display();
		object->get()->alreadyPrinted = false;
	}

	displayBackground();
	displayMapEditorPoints();
}

void MapContainer::loadWorldIntoSections(std::vector<std::unique_ptr<MapObject>>& mapObjects)
{
	mapObjectSections.clear();
	mapObjectSections.resize(100);
	mapCollidableObjectSections.clear();
	mapCollidableObjectSections.resize(100);
	highBuildingSection.clear();

	for (auto& row : mapObjectSections)
	{
		row.resize(100);
	}
	for (auto& row : mapCollidableObjectSections)
	{
		row.resize(100);
	}

	maxX = -1000000.0f;
	minX = 1000000.0f;
	maxY = -1000000.0f;
	minY = 1000000.0f;

	for (auto& mapObject : mapObjects)
	{
		for (auto& polygon : mapObject->polygons)
		{
			for (auto& point : polygon.points)
			{
				if (point.x < minX)
					minX = point.x;
				if (point.x > maxX)
					maxX = point.x;
				if (point.y < minY)
					minY = point.y;
				if (point.y > maxY)
					maxY = point.y;
			}
		}
	}

	maxX += 0.01;
	maxY += 0.01;

	deltaX = maxX - minX;
	deltaY = maxY - minY;

	addObjectsToSections(mapObjects);
	
	if(!mapObjects.empty())
		background = &mapObjects.back();

	addGroundAreas(mapObjects);
}

void MapContainer::addGroundAreas(std::vector<std::unique_ptr<MapObject>>& mapObjects)
{
	ground.clear();

	for (auto& mapObject : mapObjects)
	{
		auto ID = mapObject.get()->getId();
		if (ID == -100047 || ID == -100096 || ID == -100032 || ID == -100043 || ID == -100045 || ID == -100041 || ID == -100038)
			ground.push_back(&mapObject);
	}
}

void MapContainer::addObjectsToSections(std::vector<std::unique_ptr<MapObject>>& mapObjects)
{
	for (auto& mapObject : mapObjects)
	{
		bool broken = false;

		bool highBuilding = false;
		std::vector<std::pair<int, int>> sectionsXY;

		for (auto& polygon : mapObject->polygons)
		{
			for (auto& point : polygon.points)
			{
				int sectionX = (point.x - minX) * 100 / deltaX;
				int sectionY = (point.y - minY) * 100 / deltaY;

				if (sectionX >= 100 || sectionX < 0 || sectionY >= 100 || sectionY < 0)
					broken = true;

				sectionsXY.push_back({ sectionX, sectionY });

				if (point.z > 25)
				{
					highBuilding = true;
				}
			}
		}

		if (broken)
			continue;

		if(highBuilding)
			highBuildingSection.push_back(&mapObject);

		sort(sectionsXY.begin(), sectionsXY.end());
		sectionsXY.erase(unique(sectionsXY.begin(), sectionsXY.end()), sectionsXY.end());

		for (auto& sectionXY : sectionsXY)
		{
			mapObjectSections[sectionXY.first][sectionXY.second].push_back(&mapObject);
			if (mapObject->collidable != Collidable::no)
			{
				mapCollidableObjectSections[sectionXY.first][sectionXY.second].push_back(&mapObject);
			}
		}
	}

	for (int q = 0; q < 100; q++)
	{
		for (int w = 0; w < 100; w++)
		{
			std::sort(mapObjectSections[q][w].begin(), mapObjectSections[q][w].end(), [&](const auto& a, const auto& b) { return a->get()->drawingPriority < b->get()->drawingPriority; });
		}
	}
	
}

void MapContainer::displayAllWorld()
{
	for (auto& lineToDisplay : mapObjectSections)
	{
		for (auto& sectionToDisplay : lineToDisplay)
		{
			for (auto& object : sectionToDisplay)
			{
				object->get()->display();
				object->get()->alreadyPrinted = false;
			}
		}
	}

	displayBackground();
	displayMapEditorPoints();
}

void MapContainer::displaySector(const Point& point)
{
	int yToDraw = static_cast<int>(100 * (point.y - minY) / deltaY);
	int xToDraw = static_cast<int>(100 * (point.x - minX) / deltaX);

	std::vector<std::pair<int, int>> sectorsToDisplay;
	for (int q = -10; q <= 10; q++)
	{
		for (int w = -10; w <= 10; w++)
		{
			if(xToDraw + q >= 0 && xToDraw + q < 100 && yToDraw + w >= 0 && yToDraw + w < 100)
				sectorsToDisplay.push_back({ xToDraw + q, yToDraw + w });
		}
	}

	for (auto& sectionToDisplay : sectorsToDisplay)
	{
		if (sectionToDisplay.first < 100 && sectionToDisplay.second < 100 &&
			sectionToDisplay.first >= 0 && sectionToDisplay.second >= 0)
			for (auto& object : mapObjectSections[sectionToDisplay.first][sectionToDisplay.second])
			{
				if (!object->get()->isHidden)
					object->get()->display();
			}
	}

	for (auto& sectionToDisplay : sectorsToDisplay)
	{
		for (auto& object : mapObjectSections[sectionToDisplay.first][sectionToDisplay.second])
		{
			object->get()->alreadyPrinted = false;
		}
	}

	displayLines();
	displayBackground();
	displayMapEditorPoints();
}

void MapContainer::displayBackground()
{
	if (background != nullptr)
	{
		//background->get()->printObject = true;
		//background->get()->printTexture = true;
		background->get()->display();
		background->get()->alreadyPrinted = false;
	}

	for (const auto& groundArea : ground)
	{
		//groundArea->get()->printObject = true;
		//groundArea->get()->printTexture = true;

		groundArea->get()->display();
		groundArea->get()->alreadyPrinted = false;
	}
}

void MapContainer::displayLines()
{
	if (!linesVisible)
		return;

	glColor3f(0.0f, 0.0f, 0.0f);
	for (int xx = 0; xx < 100; xx++)
	{
		glBegin(GL_LINES);
		glVertex3f(minX + xx * deltaX / 100, minY, 0.05);
		glVertex3f(minX + xx * deltaX / 100, maxY, 0.05);
		glEnd();
	}
	for (int yy = 0; yy < 100; yy++)
	{
		glBegin(GL_LINES);
		glVertex3f(minX, minY + yy * deltaY / 100, 0.05);
		glVertex3f(maxX, minY + yy * deltaY / 100, 0.05);
		glEnd();
	}
}

void MapContainer::displayMapEditorPanel()
{
	if (mapEditorPanelVisible)
	{

		glEnable(GL_BLEND);

		glLineWidth(3);

		glBegin(GL_LINE_LOOP);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.5 * w - 0.5 * h + mapEditorSelectedPanel * 0.05 * h, 0.95 * h);
		glVertex2f(0.5 * w - 0.5 * h + (mapEditorSelectedPanel + 1) * 0.05 * h, 0.95 * h);
		glVertex2f(0.5 * w - 0.5 * h + (mapEditorSelectedPanel + 1) * 0.05 * h, h);
		glVertex2f(0.5 * w - 0.5 * h + mapEditorSelectedPanel * 0.05 * h, h);

		glEnd();

		glBegin(GL_LINE_LOOP);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.5 * w - 0.5 * h + mapEditorSelectedTool * 0.05 * h, 0.9 * h);
		glVertex2f(0.5 * w - 0.5 * h + (mapEditorSelectedTool + 1) * 0.05 * h, 0.9 * h);
		glVertex2f(0.5 * w - 0.5 * h + (mapEditorSelectedTool + 1) * 0.05 * h, 0.95 * h);
		glVertex2f(0.5 * w - 0.5 * h + mapEditorSelectedTool * 0.05 * h, 0.95 * h);

		glEnd();

		glLineWidth(1.0);

		glDisable(GL_BLEND);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		static int idTexture = TextureManager::Instance()->textures[static_cast<int>(Textures::map_editor_panel)].idTexture;

		glBindTexture(GL_TEXTURE_2D, idTexture);
		glBegin(GL_POLYGON);

		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, static_cast<float>(tools.size()) / (tools.size() + 1));
		glVertex2f(0.5 * w - 0.5 * h, 0.95 * h);
		glTexCoord2f(1, static_cast<float>(tools.size()) / (tools.size() + 1));
		glVertex2f(0.5 * w + 0.5 * h, 0.95 * h);
		glTexCoord2f(1, 1);
		glVertex2f(0.5 * w + 0.5 * h, h);
		glTexCoord2f(0, 1);
		glVertex2f(0.5 * w - 0.5 * h, h);

		glEnd();


		glBegin(GL_POLYGON);

		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, static_cast<float>(tools.size() - 1 - mapEditorSelectedPanel) / (tools.size() + 1));
		glVertex2f(0.5 * w - 0.5 * h, 0.9 * h);
		glTexCoord2f(1, static_cast<float>(tools.size() - 1 - mapEditorSelectedPanel) / (tools.size() + 1));
		glVertex2f(0.5 * w + 0.5 * h, 0.9 * h);
		glTexCoord2f(1, static_cast<float>(tools.size() - mapEditorSelectedPanel) / (tools.size() + 1));
		glVertex2f(0.5 * w + 0.5 * h, 0.95 * h);
		glTexCoord2f(0, static_cast<float>(tools.size() - mapEditorSelectedPanel) / (tools.size() + 1));
		glVertex2f(0.5 * w - 0.5 * h, 0.95 * h);

		glEnd();


		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}
}

void MapContainer::displayCounter()
{
	if (raceTimer.state >= 0)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
	
		static int idTexture = TextureManager::Instance()->textures[static_cast<int>(Textures::counter)].idTexture;

		glBindTexture(GL_TEXTURE_2D, idTexture);
		glBegin(GL_POLYGON);

		glColor3f(1.0f, 1.0f, 1.0f);

		glTexCoord2f(0.2 * raceTimer.state, 0);
		glVertex2f(0.5 * w - 0.15 * h, 0.7 * h);

		glTexCoord2f(0.2 * raceTimer.state + 0.2, 0);
		glVertex2f(0.5 * w + 0.15 * h, 0.7 * h);

		glTexCoord2f(0.2 * raceTimer.state + 0.2, 1);
		glVertex2f(0.5 * w + 0.15 * h, 0.9 * h);

		glTexCoord2f(0.2 * raceTimer.state, 1);
		glVertex2f(0.5 * w - 0.15 * h, 0.9 * h);

		glEnd();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}
}

void MapContainer::pickTool(float pX, float pY)
{
	if (pY >= 0.95 && pX >= -0.5 && pX <= 0.5)
	{
		auto nextSelectedPanel = (pX + 0.5) * 20;
		if (tools.size() >= nextSelectedPanel)
			mapEditorSelectedPanel = nextSelectedPanel;

		mapEditorSelectedTool = 0;
	}
	else if (pY >= 0.9 && pY <= 0.95 && pX >= -0.5 && pX <= 0.5)
	{
		auto nextMapEditorSelectedTool = (pX + 0.5) * 20;
		if (tools[mapEditorSelectedPanel].size() >= nextMapEditorSelectedTool)
			mapEditorSelectedTool = nextMapEditorSelectedTool;
	}

	if (tools[mapEditorSelectedPanel].size() > mapEditorSelectedTool)
	{
		if(tools[mapEditorSelectedPanel].size() > mapEditorSelectedTool)
			currentToolId = tools[mapEditorSelectedPanel][mapEditorSelectedTool];
	}
}
void MapContainer::useTool(const Point& point)
{
	auto toolIt = toolsMap.find(currentToolId);
	if (toolIt != toolsMap.end())
	{
		auto tool = toolIt->second;
		(Instance().get()->*tool)(point);
	}

}

void MapContainer::recalculateAIPointsDistances()
{
	AIPointsDistances.clear();
	if (AIPoints.size() <= 1)
	{
		AIPointsDistances.push_back(0.0f);
	}
	else
	{
		for (int q = 0; q < AIPoints.size(); q++)
		{
			AIPointsDistances.push_back(AIPoints[q].center.distance2D(AIPoints[(q + 1) % AIPoints.size()].center));
		}
	}
}

void MapContainer::addPoint(const Point& point)
{
	if(currentPath.empty() || currentPath.back().center.distance2D(point) > 0.1)
		currentPath.push_back({ point, pointsColor, false });
}

void MapContainer::addPointInTheMiddle(const Point& point)
{
	if(currentPath.empty() || currentPath.back().center.distance2D(CameraManager::Instance()->cameraViews[0]->getCameraLookAt()) > 0.1)
		currentPath.push_back({ CameraManager::Instance()->cameraViews[0]->getCameraLookAt(), pointsColor, false });
}

void MapContainer::removePoint(const Point& point)
{
	for (int q = 0; q < currentPath.size(); q++)
	{
		if (currentPath[q].selected)
		{
			currentPath.erase(currentPath.begin() + q);
			if (!currentPath.empty())
			{
				if (q >= currentPath.size())
				{
					currentPath.back().selected = true;
				}
				else
				{
					currentPath[q].selected = true;
				}
			}
			break;
		}
	}
}

void MapContainer::ConvertPathToAIPoints(const Point& point)
{
	if (!currentPath.empty())
	{
		AIPoints = currentPath;
		currentPath.clear();
		recalculateAIPointsDistances();
		std::for_each(AIPoints.begin(), AIPoints.end(), [&](PathStruct& a) {a.color = AIPointsColor; });
	}
}

void MapContainer::ConvertAIPointsToPath(const Point& point)
{
	if (!AIPoints.empty())
	{
		currentPath = AIPoints;
		AIPoints.clear();
		recalculateAIPointsDistances();
		std::for_each(currentPath.begin(), currentPath.end(), [&](PathStruct& a) {a.color = pointsColor; });
	}
}

void MapContainer::AddSplinePoint(const Point& point)
{
	currentSpline.spline.push_back(point);
}

void MapContainer::RemoveSplinePoint(const Point& point)
{
	if (currentSpline.selected >= 0 && currentSpline.selected < currentSpline.spline.size())
	{
		currentSpline.spline.points.erase(currentSpline.spline.points.begin() + currentSpline.selected);

		if (!currentSpline.spline.points.empty())
		{
			if (currentSpline.selected >= currentSpline.spline.size())
			{
				currentSpline.selected = currentSpline.spline.size() - 1;
			}
		}
		else
		{
			currentSpline.selected = -1;
		}
	}

	for (int q = 0; q < currentPath.size(); q++)
	{
		if (currentPath[q].selected)
		{
			currentPath.erase(currentPath.begin() + q);
			if (!currentPath.empty())
			{
				if (q >= currentPath.size())
				{
					currentPath.back().selected = true;
				}
				else
				{
					currentPath[q].selected = true;
				}
			}
			break;
		}
	}
}

void MapContainer::MoveSplinePoint(const Point& point)
{
	if (currentSpline.selected >= 0 && currentSpline.selected < currentSpline.spline.size())
	{
		currentSpline.spline.points[currentSpline.selected] = point;
	}
}

void MapContainer::SelectSplinePoint(const Point& point)
{
	currentSpline.selected = -1;

	for (int q = 0; q < currentSpline.spline.points.size(); q++)
	{
		if (currentSpline.spline.points[q].distance2D(point) < 1.0)
		{
			currentSpline.selected = q;
			break;
		}
	}
}

void MapContainer::RemoveSplinePoints(const Point& point)
{
	currentSpline.spline.points.clear();
	currentSpline.selected = -1;
}

void MapContainer::DivideSpline(const Point& point)
{
	currentSpline.subpointsDistance = 5.0f;
}

void MapContainer::IncreaseSplineSubpoints(const Point& point)
{
	currentSpline.subpointsDistance -= 0.5f;
}

void MapContainer::DecreaseSplineSubpoints(const Point& point)
{
	currentSpline.subpointsDistance += 0.5f;
}

void MapContainer::ConvertSplineToCurrentPath(const Point& point)
{
	if (currentSpline.spline.points.size() > 3)
	{
		currentPath = generateSubsplinePath();
		currentSpline = SplineStruct{};
	}
}

void MapContainer::ConvertSplinePointsToCurrentPath(const Point& point)
{
	std::vector<PathStruct> futurePath;
	auto& splinePoints = currentSpline.spline.points;

	if (!splinePoints.empty())
	{
		std::for_each(splinePoints.begin(), splinePoints.end(), [&](const Point& p) {futurePath.push_back({ p, splineSubointsColor, false }); });

		currentPath = std::move(futurePath);
		currentSpline = SplineStruct{};
	}
}

void MapContainer::ConvertPathToRaceBarriers(const Point& point)
{
	if (currentPath.size() > 1)
	{
		for (int q = 0; q < currentPath.size() - 1; q++)
		{
			if (currentPath[q].center.distance2D(currentPath[q + 1].center) > 1.5)
			{
				raceBarriers.push_back({ currentPath[q].center, currentPath[q + 1].center });
			}
		}
	}
}

void MapContainer::ConvertPathToRaceBarriersSaveAndClearPath(const Point& point)
{
	ConvertPathToRaceBarriers();
	SaveAIPoints();
	removePoints();
}

void MapContainer::ConvertPathToMeta(const Point& point)
{
	if (currentPath.size() == 2)
	{
		meta.first = currentPath[0].center;
		meta.second = currentPath[1].center;
	}
}

void MapContainer::AddCameraPoint(const Point& point)
{
	auto& currentCameraCenter = CameraManager::Instance()->getCurrentCameraPoints().first;
	auto& currentCameraLookAt = CameraManager::Instance()->getCurrentCameraPoints().second;

	if (cameraSpline.first.points.empty()
		|| cameraSpline.first.points.back().distance3D(currentCameraCenter) > 0.01
		|| cameraSpline.second.points.back().distance3D(currentCameraLookAt) > 0.01
		)
	{
		cameraSpline.first.points.push_back(currentCameraCenter);
		cameraSpline.second.points.push_back(currentCameraLookAt);
	}
}

void MapContainer::RemoveCameraPoints(const Point& point)
{
	cameraSpline.first.points.clear();
	cameraSpline.second.points.clear();
	CameraManager::Instance()->specialCameraPath.clear();
}

void MapContainer::ConvertCameraPointsToSpline(const Point& point)
{
	ConvertCameraPointsToSpline(false);
}

void MapContainer::ConvertCameraPointsToSplineVariouosDistances(const Point& point)
{
	ConvertCameraPointsToSpline(true);
}

void MapContainer::ConvertCameraPointsToSpline(bool variousDistances)
{
	if (cameraSpline.first.points.size() >= 4)
	{
		cameraSpline.first.calculateLengths();
		cameraSpline.second.calculateLengths();

		std::vector<Point> splineSubpointsCenter;
		std::vector<Point> splineSubpointsLookAt;

		if (variousDistances)
		{
			splineSubpointsCenter = cameraSpline.first.generateSubpointsVariousDistances(cameraSpline.first.length() / 500);
			splineSubpointsLookAt = cameraSpline.second.generateSubpointsVariousDistances(cameraSpline.second.length() / 500);
		}
		else
		{
			splineSubpointsCenter = cameraSpline.first.generateSubpoints(cameraSpline.first.length() / 500);
			splineSubpointsLookAt = cameraSpline.second.generateSubpoints(cameraSpline.second.length() / 500);
		}

		std::vector<std::pair<Point, Point>> newSpecialCameraPath;

		int limit = std::min(splineSubpointsCenter.size(), splineSubpointsLookAt.size());
		for (int q = 0; q < limit; q++)
		{
			newSpecialCameraPath.push_back({ splineSubpointsCenter[q], splineSubpointsLookAt[q] });
		}

		CameraManager::Instance()->specialCameraPath = newSpecialCameraPath;
		CameraManager::Instance()->restartIdSpecialCameraPath();
	}
}

void MapContainer::PlayCameraSpline(const Point& point)
{
	glutSetCursor(GLUT_CURSOR_NONE);
	hideMapEditorPanel();
	hideCursors();
	CameraManager::Instance()->setCarZero(nullptr);
	MapManager::Instance()->currentCameraView = -1;
}

void MapContainer::ConvertCameraSplineToCarZero(const Point& point)
{
	if (!cars.empty())
	{
		CameraManager::Instance()->setCarZero(&cars[0]);
		for (auto& cameraSet : CameraManager::Instance()->specialCameraPath)
		{
			cameraSet.first -= CameraManager::Instance()->getCarZero()->position;
			cameraSet.second -= CameraManager::Instance()->getCarZero()->position;

			Point::rotate(cameraSet.first, Point(), -CameraManager::Instance()->getCarZero()->rz);
			Point::rotate(cameraSet.second, Point(), -CameraManager::Instance()->getCarZero()->rz);
		}
	}
}

void MapContainer::IncreaseSpecialCameraSpeed(const Point& point)
{
	CameraManager::Instance()->increaseSpecialCameraSpeed();
}

void MapContainer::DecreaseSpecialCameraSpeed(const Point& point)
{
	CameraManager::Instance()->decreaseSpecialCameraSpeed();
}

void MapContainer::LoadCameraSpline(const Point& point)
{
	std::ifstream file;
	file.open("Data/CameraPoints.txt", std::ios::in);

	if (file)
	{
		CameraManager::Instance()->specialCameraPath.clear();

		double x1, y1, z1, x2, y2, z2;
		while (file >> x1 >> y1 >> z1 >> x2 >> y2 >> z2)
		{
			CameraManager::Instance()->specialCameraPath.push_back({ {x1, y1, z1 }, { x2, y2, z2 } });
		}

	}

	file.close();
}

void MapContainer::SaveCameraSpline(const Point& point)
{
	if (!CameraManager::Instance()->specialCameraPath.empty())
	{
		std::ofstream file;
		file.open("Data/CameraPoints.txt", std::ios::out);

		for (auto& cameraSet : CameraManager::Instance()->specialCameraPath)
		{
			file << cameraSet.first.x << " " << cameraSet.first.y << " " << cameraSet.first.z << " ";
			file << cameraSet.second.x << " " << cameraSet.second.y << " " << cameraSet.second.z << "\n";
		}

		file.close();
	}
}

void MapContainer::SaveNewObject(const Point& point)
{
	MapManager::Instance()->saveExtraObjects();
}

void MapContainer::DeleteLastNewObject(const Point& point)
{
	for (auto& ref : extraObjects.back().get()->refs)
	{
		MapManager::Instance()->removeExtraNode(ref);
	}

	extraObjects.pop_back();
}

void MapContainer::AddTree(const Point& point)
{
	for (auto& point : currentPath)
	{
		auto newNode = MapManager::Instance()->addNewExtraNode(point.center);
		extraObjects.push_back(std::make_unique<Tree>(Tree(newNode.id)));
		extraObjects.back()->calculateXYfromRef(MapManager::Instance()->extraNodes);
	}
}

void MapContainer::AddStreetLamp(const Point& point)
{
	for (auto& point : currentPath)
	{
		auto newNode = MapManager::Instance()->addNewExtraNode(point.center);
		extraObjects.push_back(std::make_unique<StreetLamp>(StreetLamp(newNode.id)));
		extraObjects.back()->calculateXYfromRef(MapManager::Instance()->extraNodes);
	}
}

void MapContainer::AddBuilding(const Point& point)
{
	MapObject newObject(MapManager::Instance()->lastExtraObjectId);
	MapManager::Instance()->lastExtraObjectId--;

	for (auto& point : currentPath)
	{
		auto newNode = MapManager::Instance()->addNewExtraNode(point.center);
		newObject.refs.push_back(newNode.id);
	}
	newObject.refs.push_back(newObject.refs[0]);

	extraObjects.push_back(std::make_unique<Building>(Building(newObject)));
	extraObjects.back()->calculateXYfromRef(MapManager::Instance()->extraNodes);
	extraObjects.back()->calculateFinalGeometry();
}

void MapContainer::AddBarrier(const Point& point)
{
	MapObject newObject(MapManager::Instance()->lastExtraObjectId);
	MapManager::Instance()->lastExtraObjectId--;

	for (auto& point : currentPath)
	{
		auto newNode = MapManager::Instance()->addNewExtraNode(point.center);
		newObject.refs.push_back(newNode.id);
	}

	extraObjects.push_back(std::make_unique<Barrier>(Barrier(newObject)));
	extraObjects.back()->calculateXYfromRef(MapManager::Instance()->extraNodes);
	extraObjects.back()->calculateFinalGeometry();
}

void MapContainer::AddRamp(const Point& point)
{
	MapObject newObject(MapManager::Instance()->lastExtraObjectId);
	MapManager::Instance()->lastExtraObjectId--;

	for (auto& point : currentPath)
	{
		auto newNode = MapManager::Instance()->addNewExtraNode(point.center);
		newObject.refs.push_back(newNode.id);
	}

	extraObjects.push_back(std::make_unique<Ramp>(Ramp(newObject)));
	extraObjects.back()->calculateXYfromRef(MapManager::Instance()->extraNodes);
	extraObjects.back()->calculateFinalGeometry();
}

void MapContainer::AddCustomArea(const Point& point)
{
	MapObject newObject(MapManager::Instance()->lastExtraObjectId);
	MapManager::Instance()->lastExtraObjectId--;

	for (auto& point : currentPath)
	{
		auto newNode = MapManager::Instance()->addNewExtraNode(point.center);
		newObject.refs.push_back(newNode.id);
	}

	extraObjects.push_back(std::make_unique<CustomArea>(CustomArea(newObject)));
	extraObjects.back()->calculateXYfromRef(MapManager::Instance()->extraNodes);
	extraObjects.back()->calculateFinalGeometry();
}

void MapContainer::NewObjectAddHeight(const Point& point)
{
	extraObjects.back()->_height += 1;
	extraObjects.back()->recalculateFinalGeometry();
}

void MapContainer::NewObjectReduceHeight(const Point& point)
{
	extraObjects.back()->_height -= 1;
	extraObjects.back()->recalculateFinalGeometry();
}

void MapContainer::NewObjectAddMinHeight(const Point& point)
{
	extraObjects.back()->_min_height += 1;
	extraObjects.back()->recalculateFinalGeometry();
}

void MapContainer::NewObjectReduceMinHeight(const Point& point)
{
	extraObjects.back()->_min_height -= 1;
	extraObjects.back()->recalculateFinalGeometry();
}

void MapContainer::NewObjectAddRed(const Point& point)
{
	extraObjects.back()->_color.red += 0.1;
	if (extraObjects.back()->_color.red > 1)
		extraObjects.back()->_color.red = 1;
	extraObjects.back()->recalculateFinalGeometry();
}

void MapContainer::NewObjectReduceRed(const Point& point)
{
	extraObjects.back()->_color.red -= 0.1;
	if (extraObjects.back()->_color.red < 0)
		extraObjects.back()->_color.red = 0;
	extraObjects.back()->recalculateFinalGeometry();
}

void MapContainer::NewObjectAddGreen(const Point& point)
{
	extraObjects.back()->_color.green += 0.1;
	if (extraObjects.back()->_color.green > 1)
		extraObjects.back()->_color.green = 1;
	extraObjects.back()->recalculateFinalGeometry();
}

void MapContainer::NewObjectReduceGreen(const Point& point)
{
	extraObjects.back()->_color.green -= 0.1;
	if (extraObjects.back()->_color.green < 0)
		extraObjects.back()->_color.green = 0;
	extraObjects.back()->recalculateFinalGeometry();
}

void MapContainer::NewObjectAddBlue(const Point& point)
{
	extraObjects.back()->_color.blue += 0.1;
	if (extraObjects.back()->_color.blue > 1)
		extraObjects.back()->_color.blue = 1;
	extraObjects.back()->recalculateFinalGeometry();
}

void MapContainer::NewObjectReduceBlue(const Point& point)
{
	extraObjects.back()->_color.blue -= 0.1;
	if (extraObjects.back()->_color.blue < 0)
		extraObjects.back()->_color.blue = 0;
	extraObjects.back()->recalculateFinalGeometry();
}

void MapContainer::NewObjectAddRemoveCustomTexture(const Point& point)
{
	if (extraObjects.back()->_custom_texture.empty())
		extraObjects.back()->_custom_texture = "no_texture.png";
	else
		extraObjects.back()->_custom_texture.clear();

	extraObjects.back()->recalculateFinalGeometry();
}

void MapContainer::NewObjectNextTexture(const Point& point)
{
	static std::vector<Textures> possibleCustomTextures
	{
		Textures::no_texture,
		Textures::building_00_big,
		Textures::building_01_big,
		Textures::building_02_big,
		Textures::building_03_big,
		Textures::building_04_big,
		Textures::building_05_big,
		Textures::building_06_big,
		Textures::building_07_big,
		Textures::building_08_big,
		Textures::office_windows,
	};

	if (!extraObjects.back()->_custom_texture.empty())
	{
		int nextTextureId = 0;

		auto textureName = TextureManager::Instance()->getTextureFromFilePath(extraObjects.back()->_custom_texture);

		for (int q = 0; q < possibleCustomTextures.size(); q++)
		{
			if (textureName == possibleCustomTextures[q])
			{
				nextTextureId = (q + 1) % possibleCustomTextures.size();
				break;
			}
		}

		extraObjects.back()->_custom_texture = TextureManager::Instance()->textures[static_cast<long>(possibleCustomTextures[nextTextureId])].filePath;

		extraObjects.back()->recalculateFinalGeometry();
	}
}

void MapContainer::ReloadMap(const Point& point)
{
	MapManager::Instance()->rereadMap();
}

void MapContainer::SaveMapObject(const Point& point)
{
	if (buildingEdited.object != nullptr)
	{
		buildingEdited.object->isSelected = true;

		if (buildingEdited.heightChanged)
			MapManager::Instance()->addOverlayAttribute("height", std::to_string(buildingEdited.object->_height).c_str());
		if (buildingEdited.colorChanged)
			MapManager::Instance()->addOverlayAttribute("colour", buildingEdited.object->_color.getColorHex().c_str());
		if (buildingEdited.textureChanged)
			MapManager::Instance()->addOverlayAttribute("_custom_texture", buildingEdited.object->_custom_texture.c_str());

		buildingEdited.object->isSelected = false;

		buildingEdited = BuildingEdited();
	}

	MapManager::Instance()->saveOverlays();
}

void MapContainer::EditMapObject(const Point& point)
{
	buildingEdited.object = nullptr;

	for (auto& mapObject : MapManager::Instance()->mapObjects)
	{
		if (buildingEdited.object == nullptr && mapObject->isSelected
			&& (MapManager::Instance()->isBuildingCheck(*mapObject.get()) || MapManager::Instance()->isBarrierCheck(*mapObject.get())))
		{
			buildingEdited.object = mapObject.get();
		}

		mapObject->deselect();
	}
}

void MapContainer::EditMapObjectNextBuildingTexture(const Point& point)
{
	static std::vector<Textures> possibleCustomTextures
	{
		Textures::no_texture,
		Textures::building_00_big,
		Textures::building_01_big,
		Textures::building_02_big,
		Textures::building_03_big,
		Textures::building_04_big,
		Textures::building_05_big,
		Textures::building_06_big,
		Textures::building_07_big,
		Textures::building_08_big,
		Textures::office_windows,
	};

	if (buildingEdited.object != nullptr)
	{
		buildingEdited.textureChanged = true;

		int nextTextureId = 0;

		auto textureName = TextureManager::Instance()->getTextureFromFilePath(buildingEdited.object->_custom_texture);

		for (int q = 0; q < possibleCustomTextures.size(); q++)
		{
			if (textureName == possibleCustomTextures[q])
			{
				nextTextureId = (q + 1) % possibleCustomTextures.size();
				break;
			}
		}

		buildingEdited.object->_custom_texture = TextureManager::Instance()->textures[static_cast<long>(possibleCustomTextures[nextTextureId])].filePath;

		buildingEdited.object->recalculateFinalGeometry();
	}
}

void MapContainer::EditMapObjectNextRoofTexture(const Point& point)
{

}

void MapContainer::EditMapObjectDoNotGenerateBottomRoof(const Point& point)
{

}

void MapContainer::EditMapObjectChangeRoofType(const Point& point)
{

}

void MapContainer::EditMapObjectAddHeight(const Point& point)
{
	if (buildingEdited.object != nullptr)
	{
		buildingEdited.heightChanged = true;
		buildingEdited.object->_height += 1;
		buildingEdited.object->recalculateFinalGeometry();
	}
}

void MapContainer::EditMapObjectReduceHeight(const Point& point)
{
	if (buildingEdited.object != nullptr)
	{
		buildingEdited.heightChanged = true;
		buildingEdited.object->_height -= 1;
		buildingEdited.object->recalculateFinalGeometry();
	}
}

void MapContainer::EditMapObjectAddRed(const Point& point)
{
	if (buildingEdited.object != nullptr)
	{
		buildingEdited.colorChanged = true;

		buildingEdited.object->_color.red += 0.1;
		if (buildingEdited.object->_color.red > 1)
			buildingEdited.object->_color.red = 1;
		buildingEdited.object->recalculateFinalGeometry();
	}
}

void MapContainer::EditMapObjectReduceRed(const Point& point)
{
	if (buildingEdited.object != nullptr)
	{
		buildingEdited.colorChanged = true;

		buildingEdited.object->_color.red -= 0.1;
		if (buildingEdited.object->_color.red < 0)
			buildingEdited.object->_color.red = 0;
		buildingEdited.object->recalculateFinalGeometry();
	}
}

void MapContainer::EditMapObjectAddGreen(const Point& point)
{
	if (buildingEdited.object != nullptr)
	{
		buildingEdited.colorChanged = true;

		buildingEdited.object->_color.green += 0.1;
		if (buildingEdited.object->_color.green > 1)
			buildingEdited.object->_color.green = 1;
		buildingEdited.object->recalculateFinalGeometry();
	}
}

void MapContainer::EditMapObjectReduceGreen(const Point& point)
{
	if (buildingEdited.object != nullptr)
	{
		buildingEdited.colorChanged = true;

		buildingEdited.object->_color.green -= 0.1;
		if (buildingEdited.object->_color.green < 0)
			buildingEdited.object->_color.green = 0;
		buildingEdited.object->recalculateFinalGeometry();
	}
}

void MapContainer::EditMapObjectAddBlue(const Point& point)
{
	if (buildingEdited.object != nullptr)
	{
		buildingEdited.colorChanged = true;

		buildingEdited.object->_color.blue += 0.1;
		if (buildingEdited.object->_color.blue > 1)
			buildingEdited.object->_color.blue = 1;
		buildingEdited.object->recalculateFinalGeometry();
	}
}

void MapContainer::EditMapObjectReduceBlue(const Point& point)
{
	if (buildingEdited.object != nullptr)
	{
		buildingEdited.colorChanged = true;

		buildingEdited.object->_color.blue -= 0.1;
		if (buildingEdited.object->_color.blue < 0)
			buildingEdited.object->_color.blue = 0;
		buildingEdited.object->recalculateFinalGeometry();
	}
}

void MapContainer::PlayCameraSplineAroundCarZero(const Point& point)
{
	glutSetCursor(GLUT_CURSOR_NONE);
	hideMapEditorPanel();
	hideCursors();
	if (!cars.empty())
	{
		CameraManager::Instance()->setCarZero(&cars[0]);
		MapManager::Instance()->currentCameraView = -1;
	}
}

void MapContainer::resetCarPositionsToPoint(int idPoint)
{
	vector2D direction(AIPoints[idPoint + 1].center, AIPoints[idPoint].center);
	direction.convertIntoUnitVector();
	float newRz = vector2D::directedAngle(vector2D({ 0, 0 }, { 1, 0 }), direction) + PI;

	vector2D perpendicular(direction);
	perpendicular.rotate(PI / 2);

	Point firstCarPosition = AIPoints[idPoint].center;

	int index = 0;
	for (auto& car : MapContainer::Instance()->cars)
	{
		Point carPosition = firstCarPosition;
		carPosition.move2D(index * 5 * direction.x, index * 5 * direction.y);
		carPosition.move2D(((index % 2) * 2 - 1) * perpendicular.x * 2, ((index % 2) * 2 - 1) * perpendicular.y * 2);
		car.setPosition(carPosition, newRz);
		car.setAIcurrentPoint(idPoint);
		index++;
	}

}

void MapContainer::AIStop(const Point& point)
{
	AIPause();

	if (AIPoints.size() > 1)
	{
		resetCarPositionsToPoint(0);
	}

	raceTimer.resetTimer();
}

void MapContainer::AIStopAndRestartToSelectedPoint(const Point& point)
{
	AIPause();

	int selectedPoint = getSelectedPointIndex();

	if (AIPoints.size() > selectedPoint + 1)
	{
		resetCarPositionsToPoint(selectedPoint);
	}

	raceTimer.resetTimer();
}

void MapContainer::setFreeRide(CarBrand selectedCar, Point position)
{
	initCars(selectedCar, 0);
	cars.back().setPosition(position, 0);

	AIPoints.clear();
	raceBarriers.clear();
	LoadRaceStartCameraPoints();
	MapManager::Instance()->currentCameraView = -1;
	AIStop();
	raceTimer.state = RaceTimer::State::Intro;
	raceTimer.beforeRace = true;

	loadWorldIntoSections(MapManager::Instance()->mapObjects);
}

void MapContainer::setRaceDetails(CarBrand selectedCar, TrackName selectedTrack, int noOfLaps, int noOfOponents)
{
	raceTimer.setMaxNoOfLaps(noOfLaps);
	initCars(selectedCar, noOfOponents);

	LoadAIPointsFromTrackName(selectedTrack);
	LoadRaceStartCameraPoints();

	MapManager::Instance()->currentCameraView = -1;
	AIStop();
	raceTimer.state = RaceTimer::State::Intro;
	raceTimer.beforeRace = true;

	loadWorldIntoSections(MapManager::Instance()->mapObjects);
	createRaceObjects();
}

void MapContainer::startRace(const Point& point)
{
	LoadAIPoints();
	LoadRaceStartCameraPoints();

	MapManager::Instance()->currentCameraView = -1;
	AIStop();
	raceTimer.state = RaceTimer::State::Intro;
	raceTimer.beforeRace = true;

	createRaceObjects();
}

void MapContainer::createRaceObjects()
{
	MapManager::Instance()->raceObjects.clear();

	for (auto& raceBarrier : raceBarriers)
	{
		MapObject raceBarierObject;
		raceBarierObject.calculateXYfromPoints(std::vector<Point>{raceBarrier.first, raceBarrier.second});
		raceBarierObject.barrier = "race_barrier";
		Barrier raceBarier(raceBarierObject);
		raceBarier.calculateFinalGeometry();

		MapManager::Instance()->raceObjects.push_back(std::make_unique<Barrier>(Barrier(raceBarier)));
	}

	MapObject metaObject;
	metaObject.calculateXYfromPoints(std::vector<Point>{meta.first, meta.second});
	MetaLine metaLine(metaObject);
	meta = metaLine.getMetaLinePoints();

	MapManager::Instance()->raceObjects.push_back(std::make_unique<Crossing>(Crossing(metaLine)));

	MapContainer::addObjectsToSections(MapManager::Instance()->raceObjects);
}

int MapContainer::getSelectedPointIndex()
{
	int index = 0;
	for (auto& AIPoint : AIPoints)
	{
		if (AIPoint.selected)
		{
			return index;
		}
		index++;
	}

	return 0;
}

void MapContainer::selectPoint(const Point& point)
{
	for (auto& currentPoint : currentPath)
	{
		currentPoint.selected = false;
	}

	for (auto& currentPoint : currentPath)
	{
		if (currentPoint.center.distance2D(point) < 1.0)
		{
			currentPoint.selected = true;
			break;
		}
	}
}

void MapContainer::movePoint(const Point& point)
{
	for (auto& currentPoint : currentPath)
	{
		if (currentPoint.selected == true)
		{
			currentPoint.center = point;
			break;
		}
	}
}

void MapContainer::AIPause(const Point& point)
{
	AIPathActive = false;
	for (auto& car : MapContainer::Instance()->cars)
	{
		car.stop();
	}
	MapContainer::Instance()->raceTimer.resetTimer();
	MapContainer::Instance()->raceTimer.beforeRace = true;
}

void MapContainer::AIPlay(const Point& point)
{
	MapContainer::Instance()->cars[0].setLocalV({ 50, 0 });
	//MapContainer::Instance()->cars[0].updateAngularVelocity(-2);

	//MapContainer::Instance()->cars[1].setLocalV({ 40, 0});

	updateVelocityOfCars();

	//MapContainer::Instance()->cars[0].setPosition({ MapContainer::Instance()->cars[0].position.x - 1, MapContainer::Instance()->cars[0].position.y - 1 }, MapContainer::Instance()->cars[1].rz);
	//MapContainer::Instance()->cars[1].setPosition({0,0}, MapContainer::Instance()->cars[1].rz);

	AIPathActive = true;
	raceTimer.startTimer();
	raceTimer.state = RaceTimer::WaitingToDisappear;
	MapContainer::Instance()->raceTimer.beforeRace = false;
}

void MapContainer::removePoints(const Point& point)
{
	currentPath.clear();
}

void MapContainer::randomizePoints(const Point& point)
{
	std::vector<Point>polygon;
	std::transform(currentPath.begin(), currentPath.end(), std::back_inserter(polygon), [&](const auto& p) { return p.center; });

	auto xMin = std::min_element(polygon.begin(), polygon.end(), [](const auto& p1, const auto& p2) { return p1.x < p2.x; });
	auto xMax = std::max_element(polygon.begin(), polygon.end(), [](const auto& p1, const auto& p2) { return p1.x < p2.x; });
	auto yMin = std::min_element(polygon.begin(), polygon.end(), [](const auto& p1, const auto& p2) { return p1.y < p2.y; });
	auto yMax = std::max_element(polygon.begin(), polygon.end(), [](const auto& p1, const auto& p2) { return p1.y < p2.y; });
	
	Point middlePoint((xMin->x + xMax->x) / 2, (yMin->y + yMax->y) / 2);

	const int numberOfRandomPoints = middlePoint.distance2D(point);

	long xRange = xMax->x - xMin->x;
	long yRange = yMax->y - yMin->y;

	for (int q = 0; q < numberOfRandomPoints; q++)
	{
		Point newPoint(xMin->x + rand() % xRange, yMin->y + rand() % yRange);

		if (PointInsidePolygonDetector::isInside(polygon, newPoint)
			&& std::all_of(
				currentSpline.spline.points.begin(),
				currentSpline.spline.points.end(),
				[&newPoint](const auto& p) { return p.distance2D(newPoint) > 1.0; }))
		{
			currentSpline.spline.push_back(newPoint);
		}
	}
}

void MapContainer::displayPath(const std::vector<PathStruct>& path, const Color& color)
{
	glPointSize(10.0);
	for (auto& pathPoint : path)
	{
		if (pathPoint.selected == true)
			glPointSize(15.0);

		glColor3f(pathPoint.color.red, pathPoint.color.green, pathPoint.color.blue);

		glBegin(GL_POINTS);
		glVertex3f(pathPoint.center.x, pathPoint.center.y, pathPoint.center.z);
		glEnd();
		glPointSize(10.0);
	}
	glPointSize(1.0);

	glColor3f(color.red, color.green, color.blue);
	for (int q = 0; q < static_cast<int>(path.size()) - 1; q++)
	{
		glBegin(GL_LINES);
		glVertex3f(path[q].center.x, path[q].center.y, 0.05);
		glVertex3f(path[q + 1].center.x, path[q + 1].center.y, 0.05);
		glEnd();
	}
}

void MapContainer::displayCurrentPath()
{
	displayPath(currentPath, pointsColor);
}

void MapContainer::displayAIPoints()
{
	displayPath(AIPoints, AIPointsColor);
}

void MapContainer::displayMapEditorPoints()
{
	//if(!mapEditorPanelVisible)
	//	return;

	for (auto& extraObject : extraObjects)
	{
		extraObject->display();
		extraObject->alreadyPrinted = false;
	}

	displayAIPoints();
	displayCurrentPath();
	displayCurrentSpline();
}

void MapContainer::displayCurrentSpline()
{
	std::vector<PathStruct> splinePath;
	std::for_each(currentSpline.spline.points.begin(), currentSpline.spline.points.end(), [&](const Point& p)
	{
		splinePath.push_back({p, splinePointsColor, false });
	});

	if (currentSpline.selected >= 0 && currentSpline.selected < currentSpline.spline.size())
	{
		splinePath[currentSpline.selected].selected = true;
	}

	displayPath(splinePath, splinePointsColor);

	if (currentSpline.subpointsDistance > 0.1f && currentSpline.spline.size() > 3)
	{
		std::vector<PathStruct> subsplinePath = generateSubsplinePath();
		displayPath(subsplinePath, splineSubointsColor);
	}
}

std::vector<MapContainer::PathStruct> MapContainer::generateSubsplinePath(bool keepOriginalPoints)
{
	currentSpline.spline.calculateLengths();

	std::vector<PathStruct> subsplinePath;
	auto splineSubpoints = currentSpline.spline.generateSubpoints(currentSpline.subpointsDistance, keepOriginalPoints);

	std::for_each(splineSubpoints.begin(), splineSubpoints.end(), [&](const Point& p) {subsplinePath.push_back({p, splineSubointsColor, false }); });

	return subsplinePath;
}

void MapContainer::SetFuturePoints(const int& futurePoint)
{
	AIPoints[futurePoint].color = Color(ColorName::PINK);
}

void MapContainer::ClearFuturePoints()
{
	for (auto& AIPoint : AIPoints)
	{
		AIPoint.color = AIPointsColor;
	}
}

void MapContainer::SaveAIPoints(const Point& point)
{
	std::ofstream file;
	file.open("Data/AIPoints.txt", std::ios::out);

	Point previous(-1000000, -100000);

	for (auto& AIPoint : AIPoints)
	{
		if (AIPoint.center.distance2D(previous) > 1)
		{
			file << "p " << AIPoint.center.x << " " << AIPoint.center.y << " " << AIPoint.center.z << "\n";
			previous = AIPoint.center;
		}
	}

	for (auto& raceBarrier : raceBarriers)
	{
		file << "b " << raceBarrier.first.x << " " << raceBarrier.first.y << " " << raceBarrier.first.z << " " << raceBarrier.second.x << " " << raceBarrier.second.y << " " << raceBarrier.second.z << "\n";
	}

	file << "m " << meta.first.x << " " << meta.first.y << " " << meta.first.z << " " << meta.second.x << " " << meta.second.y << " " << meta.second.z << "\n";

	file.close();
}

void MapContainer::LoadAIPointsFromTrackName(TrackName selectedTrack)
{
	Track newTrack(selectedTrack);

	AIPoints.clear();
	raceBarriers.clear();
	
	auto newTrackAIPoints = newTrack.getAIPoints();
	for (auto& newTrackAIPoint : newTrackAIPoints)
	{
		AIPoints.push_back({ newTrackAIPoint, AIPointsColor, false });
	}
	raceBarriers = newTrack.getBarrierPoints();	
	meta = newTrack.getMeta();
	lapRecord = newTrack.getLapRecord();

	recalculateAIPointsDistances();

	std::vector<Point> AIpointsPositions;
	std::for_each(AIPoints.begin(), AIPoints.end(), [&](const PathStruct& data) {AIpointsPositions.push_back(data.center); });

	raceTimer.setAIpointsPosition(AIpointsPositions);
	raceTimer.setLapRecord(lapRecord);
	raceTimer.setTrackName(selectedTrack);
}

void MapContainer::LoadAIPoints(const Point& point)
{
	std::ifstream file;
	file.open("Data/AIPoints.txt", std::ios::out);

	if (file)
	{
		AIPoints.clear();
		raceBarriers.clear();
		char type;
		while (file >> type)
		{
			if (type == 'p')
			{
				float x, y, z;
				file >> x >> y >> z;
				Point newPoint(x, y, z);
				AIPoints.push_back({ newPoint, AIPointsColor, false });
			}
			else if (type == 'b')
			{
				float x1, y1, z1;
				float x2, y2, z2;
				file >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
				Point newPoint1(x1, y1, z1);
				Point newPoint2(x2, y2, z2);
				raceBarriers.push_back({ newPoint1, newPoint2 });
			}
			else if (type == 'm')
			{
				float x1, y1, z1;
				float x2, y2, z2;
				file >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
				Point newPoint1(x1, y1, z1);
				Point newPoint2(x2, y2, z2);
				meta = { newPoint1, newPoint2 };
			}
		}

		recalculateAIPointsDistances();
	}

	file.close();


	std::vector<Point> AIpointsPositions;
	std::for_each(AIPoints.begin(), AIPoints.end(), [&](const PathStruct& data) {AIpointsPositions.push_back(data.center); });

	raceTimer.setAIpointsPosition(AIpointsPositions);

	createRaceObjects();
}

void MapContainer::LoadRaceStartCameraPoints()
{
	std::ifstream file;
	file.open("Data/RaceStartCameraPoints.txt", std::ios::out);

	if (file)
	{
		if (!cars.empty())
			CameraManager::Instance()->setCarZero(&cars[0]);

		CameraManager::Instance()->restartIdSpecialCameraPath();

		CameraManager::Instance()->specialCameraPath.clear();

		float x1, y1, z1, x2, y2, z2;
		while (file >> x1 >> y1 >> z1 >> x2 >> y2 >> z2)
		{
			CameraManager::Instance()->specialCameraPath.push_back({Point(x1, y1, z1), Point(x2, y2, z2)});
		}
	}

	file.close();
}

void MapContainer::initCars()
{
	cars = {
		Car(CarBrand::SubaruBRZ, 0, 0, false),
		//Car(CarBrand::VolkswagenGolf, -5, -5, true),
		//Car(CarBrand::CitroenC3, -10, -10, true),
		//Car(CarBrand::HondaJazz, -15, -15, true),
		//Car(CarBrand::LamborghiniHuracan, -20, -20)
	};

	for (auto& car : cars)
	{
		CameraManager::Instance()->cameraViews.push_back(&car);
	}

	resetCameraViews();
}

void MapContainer::initCars(CarBrand selectedCar, int noOfOponents)
{
	cars.clear();
	cars.push_back(Car(selectedCar, 0, 0, true));

	int randomCar;
	static auto checkUnique = [](const CarBrand& carbrand) { return std::any_of(cars.begin(), cars.end(), [&](const Car& car) { return car.getCarBrand() == carbrand; }); };

	for (int q = 0; q < noOfOponents; ++q)
	{
		do
		{
			randomCar = rand() % carDB.size();
		} while (checkUnique(static_cast<CarBrand>(randomCar)));
	
		cars.push_back(Car(static_cast<CarBrand>(randomCar), 0, 0));
	}

	resetCameraViews();
}

void MapContainer::resetCameraViews()
{
	CameraManager::Instance()->cameraViews.resize(1);
	for (auto& car : cars)
	{
		CameraManager::Instance()->cameraViews.push_back(&car);
	}
}

void MapContainer::initRaceTimer()
{
	raceTimer.init(&cars);
}

void MapContainer::updateRaceTimer()
{
	raceTimer.update();

	if (raceTimer.state == RaceTimer::State::Red3)
	{
		SoundManager::Instance()->playSample(Sounds::race_timer);
	}
	else if (raceTimer.state == RaceTimer::State::Green)
	{
		Instance().get()->AIPlay();
		raceTimer.beforeRace = false;
		raceTimer.resetTimer();
		raceTimer.startTimer();
		raceTimer.state = RaceTimer::State::WaitingToDisappear;
	}

}

void MapContainer::displayRaceTimer()
{
	raceTimer.display();
}

void MapContainer::updateVelocityOfCars(float scale)
{
	for (auto& car : MapContainer::Instance()->cars)
	{
		car.updateVelocity(scale);
	}
}

void MapContainer::ApplySpecialKey()
{


	//updateVelocityOfCars(0.5f);
	//std::swap(cars[0], cars[1]);
	//std::swap(cars[1], cars[2]);
	//std::swap(cars[2], cars[3]);
}

void MapContainer::updateScenery(MapObject& mapObject, float dist)
{
	if (trailerScenery > 24)
		trailerScenery = 24;


	static bool x11 = true;

	switch (trailerScenery)
	{

	case 24:
		linesVisible = false;

	case 23:
		mapObject.printObject = true;
		mapObject.printTexture = true;
		break;

	case 22:
		if (mapObject.highway == "street_lamp" && dist < 100)
		{
			mapObject.printObject = true;
			mapObject.printTexture = true;
		}

	case 21:
		if (mapObject.natural == "tree" && dist < 100)
		{
			mapObject.printObject = true;
			mapObject.printTexture = true;
		}

	case 20:
		if (MapManager::Instance()->isBusShelterCheck(mapObject) && dist < 100)
		{
			mapObject.printObject = true;
			mapObject.printTexture = true;
		}

	case 19:
		if (MapManager::Instance()->isGreenAreaCheck(mapObject) && dist < 100)
		{
			mapObject.printObject = true;
			mapObject.printTexture = true;
		}

	case 18:
		if (mapObject.building == "roof_generated" && dist < 100)
		{
			mapObject.printTexture = true;
		}

	case 17:
		if (mapObject.building == "roof_generated" && dist < 5)
		{
			mapObject.printTexture = true;
		}

	case 16:
		if (MapManager::Instance()->isBuildingCheck(mapObject) && dist < 100)
		{
			mapObject.printTexture = true;
		}

	case 15:
		if (MapManager::Instance()->isBuildingCheck(mapObject) && dist < 5)
		{
			mapObject.printTexture = true;
		}

	case 14:
		if (MapManager::Instance()->isBuildingCheck(mapObject) && dist < 3)
		{
			mapObject.printTexture = true;
		}

	case 13:
		cars[0].prin3DtModel = true;

	case 12:
		cars[0].printWheels = true;

	case 11:
		if(x11)
			std::cout << "11";
		x11 = false;
		
	case 10:
		if (MapManager::Instance()->isFootwayCheck(mapObject) && !MapManager::Instance()->isCrossingCheck(mapObject) && dist < 100)
		{
			mapObject.printObject = true;
		}

	case 9:
		if (MapManager::Instance()->isBarrierCheck(mapObject) && dist < 100)
		{
			mapObject.printObject = true;
		}

	case 8:
		if (MapManager::Instance()->isStreetCheck(mapObject) && dist < 100)
		{
			mapObject.printObject = true;
		}

	case 7:
		if (mapObject.building == "roof_generated" && dist < 100
			|| MapManager::Instance()->isBuildingCheck(mapObject) && dist < 100)
		{
			mapObject.printObject = true;
		}
		break;

	case 6:
		if (mapObject.building == "roof_generated" && dist < 10
			|| MapManager::Instance()->isBuildingCheck(mapObject) && dist < 10)
		{
			mapObject.printObject = true;
		}
		break;

	case 5:
		if (mapObject.building == "roof_generated" && dist < 4)
		{
			mapObject.printObject = true;
		}

	case 4:
		if (MapManager::Instance()->isBuildingCheck(mapObject) && dist < 4)
		{
			mapObject.printObject = true;
		}
		break;

	case 3:
		if (MapManager::Instance()->isBuildingCheck(mapObject) && dist < 3)
		{
			mapObject.printObject = true;
		}
		break;

	case 2:
		if (MapManager::Instance()->isBuildingCheck(mapObject) && dist < 2)
		{
			mapObject.printObject = true;
		}
		break;

	case 1:
		if (MapManager::Instance()->isBuildingCheck(mapObject) && dist < 1)
		{
			mapObject.printObject = true;
		}

		break;
	case 0:
		mapObject.printObject = false;
		mapObject.printTexture = false;

		cars[0].prin3DtModel = false;
		cars[0].printWheels = false;

	}
	
}